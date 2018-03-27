using System;
using System.Text;
using System.Net.Sockets;
using System.Net;
using server;
using static server.Protocol;
using System.Collections.Generic;
using System.Timers;
using System.Threading;

namespace MultiClient
{
    class Program
    {
        /*
         * ================================================================================================
         * DEKLARACJE
         * ================================================================================================
         */
        //private static readonly Socket ClientSocket = new Socket
        //    (AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        private static readonly UdpClient ClientSocket = new UdpClient();
        private const int PORT = 36666;
        private static int sessionID = 0;
        private static int _bufferSize = 1024;
        public static int nrSekw = 1;
        public static bool start = false;
        public static bool running = true;
        public static int liczbaPakietowWyslanych = 0;
        public static int liczbaPakietowPotwierdzen = 0;
        private static byte[] _buffer = null;
        private static List<Protocol> niepotwierdzonePakiety = new List<Protocol>();
        private static System.Timers.Timer timRetransmisja;
        private static EndPoint EP = new IPEndPoint(IPAddress.Parse("127.0.0.1"), PORT);

        /*
         * ================================================================================================
         * DEKLARACJE - KONIEC
         * ================================================================================================
         */

        private static void ReceiveCallBack(IAsyncResult AR) //glowna funkcja
        {
            int dataLen = 0;
            dataLen = ClientSocket.Client.EndReceiveFrom(AR, ref EP);
            //dataLen = ClientSocket.EndReceiveFrom(AR, ref EP);
            Thread.Sleep(15);
            //Console.WriteLine(Encoding.ASCII.GetString(_buffer, 0, dataLen));
            Protocol packet = Odbior(dataLen);
            //ClientSocket.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref EP, ReceiveCallBack, EP);
            ClientSocket.Client.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref EP, ReceiveCallBack, EP);

            //jezeli to bedzie Czas do konca to nie ma zadnego senda tylko trzeba wkleic 


            //var buffer = new byte[_bufferSize];
            //int received = ClientSocket.Receive(buffer, SocketFlags.None);
            //if (received == 0) return;
            //var data = new byte[received];
            //Array.Copy(buffer, data, received);
            ////data przechowuje pakiet
            //Protocol packet = new Protocol();
            //packet.Deserialize(data);
            if (packet.GetOP() != Operacja.Potwierdz)
            {
                SendAcknowledge(packet.GetNS(),packet.GetID());
            }
            else
            {
                //JEZELI OTRZYMALISMY POTWIERDZENIE USUWAMY WPIS Z TABLICY NIEPOTWIERDZONYCH PAKIETOW
              
                           // Console.WriteLine("Potwierdzenie serwera");
                            niepotwierdzonePakiety.RemoveAll(wpis => wpis.GetNS()== packet.GetNS());

                        
             
            }
            if (packet.GetOP() == Operacja.Nawiaz && sessionID==0)
            {
                if (packet.GetOD() == Odpowiedz.OK)
                {
                    sessionID = packet.GetDA();
                    Console.WriteLine("Otrzymales ID " + packet.GetDA().ToString());
                    Console.WriteLine("Czekaj na rozpoczecie rozgrywki.");
                }
                else if (packet.GetOD() == Odpowiedz.JuzRozpoczeto)
                {
                    Console.WriteLine("Nie mozna dolaczyc, poniewaz gra wlasnie toczy. Poczekaj za "+packet.GetDA()+" sekund");
                }
            }

            else if (packet.GetOP() == Operacja.CzyOdgadnieta && start==true    )
            {
                if (packet.GetOD() == Odpowiedz.Nie)
                {
                    Console.WriteLine("Zaden klient nie zdazyl odgadnac liczby");
                }
                else if (packet.GetOD() == Odpowiedz.Tak)
                {
                    if (packet.GetDA() == 0)
                    {
                        Console.WriteLine("Ktos inny zdazyl odgadnac przed Toba");
                    }
                    if (packet.GetDA() == 1)
                    {
                        Console.WriteLine("Brawo! Zgadles liczbe!");
                    }
                }
                else if (packet.GetOD() == Odpowiedz.ZaDuza)
                {
                    Console.WriteLine("Podaj mniejsza liczbe");
                    
                }
                else if (packet.GetOD() == Odpowiedz.ZaMala)
                {
                    Console.WriteLine("Podaj wieksza liczbe");
                    
                }
            }

            else if (packet.GetOP() == Operacja.Stop && start==true)
            {
                Console.WriteLine("Liczba ktora byla do odgadniecia to: " + packet.GetDA());

                if (packet.GetOD() == Odpowiedz.Koniec)
                {
                    Console.WriteLine("Rozgrywka zostala zakonczona!");
                }
                if (packet.GetOD() == Odpowiedz.BladSerwera)
                {
                    Console.WriteLine("Wystapil blad po stronie serwera i rozgrywka zostala zakonczona");
                }
                SendDisconnection();
                Reset();
            }

            else if (packet.GetOP() == Operacja.Start && start==false)
            {
                if (start == true)
                {
                    Console.WriteLine("Blad, otrzymano drugi pakiet start");
                }
                else
                {

                    Console.WriteLine("Rozgrywka zostala rozpoczeta! Sprobuj zgadnij liczbe.");
                    Console.WriteLine("Ilosc graczy w rozgrywce: " + packet.GetDA());
                    
                   
                    start = true;
                }
            }

            else if (packet.GetOP() == Operacja.CzasDoKonca && start==true)
            {
                if (packet.GetDA() > 0)
                {
                    Console.WriteLine("Pozostalo Ci " + packet.GetDA().ToString() + " sekund");
                }
                else if (packet.GetDA() == 0)
                {
                    Console.WriteLine("Skonczyl sie czas na odgadniecie liczb!");
                }
                else
                {
                    Console.WriteLine("Blad odbioru pakietu od serwera, wartosc danych mniejsza od 0");
                }
            }
            

        }
   
        private static Protocol Odbior(int dataLen)
        {
            byte[] data = null;
            data = new byte[dataLen];
            Array.Copy(_buffer, data, dataLen);
            Protocol receivedpacket = new Protocol();
            receivedpacket.Deserialize(data);
            return receivedpacket;
        }
        
        private static void Reset()
        {
            start = false;
            sessionID = 0;
            nrSekw = 1;
            Console.WriteLine("Reset");
            timRetransmisja.Stop();
            NawiazPolaczenie();
        }

        private static void NawiazPolaczenie()
        {
            string wartosc;
            Console.WriteLine("Czy chcesz nawiazac polaczenie z serwerem? (T/N)");
            wartosc = Convert.ToString(Console.ReadLine());
            if (wartosc.Equals("T"))
            {
                timRetransmisja = new System.Timers.Timer(); //Tworzenie obiektu
                timRetransmisja.Interval = 5000; //Ustawienie przerywania na 1000ms (1s)
                timRetransmisja.Elapsed += RetransmisjaPakietow; //Przypisanie metody
                timRetransmisja.AutoReset = true;
                timRetransmisja.Start(); //Start timera
                timRetransmisja.Enabled = true;
                //ClientSocket.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref EP, ReceiveCallBack, EP);
                ClientSocket.Client.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref EP, ReceiveCallBack, EP);
                SendConnection();

                        }
            else
            {
                running = false;
                Environment.Exit(0);
            }
        }



        /*
        * ================================================================================================
        * FUNKCJA MAIN
        * GLOWNA FUNKCJA PROGRAMU KLIENCKIEGO
        * ================================================================================================
        */

        static void Main()
        {
            Console.Title = "Client";
            _buffer = new byte[_bufferSize];
            var EndPoint = new IPEndPoint(IPAddress.Any, 0);

            ClientSocket.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            ClientSocket.Client.Bind(new System.Net.IPEndPoint(System.Net.IPAddress.Any, 0));
            Loop();
            Exit();
            Environment.Exit(0);
        }
        static void Loop()
        {
            NawiazPolaczenie();
            while (running == true) {
                if (start == true)
                {
                    Zgadnij();
                }
            }
            Console.ReadKey();
        }
        /*
        * ================================================================================================
        * FUNKCJA MAIN - KONIEC
        * ================================================================================================
        */

        /// <summary>
        /// Close socket and exit program.
        /// </summary>
        private static void Exit()
        {
            SendDisconnection();
            ClientSocket.Close();
            Environment.Exit(0);
        }

        public static void SendPacket(Protocol packet)
        {
           
            if (packet.GetOP() != Operacja.Potwierdz)
            {
                packet.SetID(sessionID);
                packet.SetNS(nrSekw);
                nrSekw++;
                niepotwierdzonePakiety.Add(packet);
            }
            byte[] datagram = packet.Serialize();

            //ClientSocket.SendTo(datagram, EP);
            ClientSocket.Client.SendTo(datagram, EP);
            Thread.Sleep(30);
        }

        public static void SendAcknowledge(int nrSekw, int ID)
        {
            Protocol packet = new Protocol();
            packet.SetAckowlegement(nrSekw);
            packet.SetID(ID);
            SendPacket(packet);
        }

        public static void SendQuestion(int liczba)
        {
            Protocol packet = new Protocol();
            packet.SetDA(liczba);
            packet.SetOP(Operacja.Zgadnij);
            SendPacket(packet);
        }

        public static void SendConnection()
        {
            Protocol packet = new Protocol();
            packet.SetOP(Operacja.Nawiaz);
            SendPacket(packet);
        }

        public static void SendDisconnection()
        {
            Console.WriteLine("Rozlaczono");
            Protocol packet = new Protocol();
            packet.SetOP(Operacja.Rozlacz);
            SendPacket(packet);
        }

        /*
        * ================================================================================================
        * KONIEC FUNKCJI WYSYŁAJĄCEJ
        * ================================================================================================
        */


        public static void Zgadnij()
        {
            while (start == true)
            {
                int number = 0;
                Console.WriteLine("Wpisz liczbe do zgadniecia (wieksza od 0):");
                try
                {
                    number = Convert.ToInt32(Console.ReadLine());
                }
                catch (Exception e)
                {
                    Console.WriteLine("Niepoprawna liczba");
                   

                }
                finally
                {
                    if (start == true)
                    {
                        SendQuestion(number);
                    }
                    else
                    {
                        Console.WriteLine("Rozgrywka zostala juz zakonczona.");
                    }
                }
            }
        }
        private static void RetransmisjaPakietow(Object source, ElapsedEventArgs e)
        {

            foreach (Protocol wpis in niepotwierdzonePakiety)
            {
                Console.WriteLine("Retransmisja pakietu: NS-" + wpis.GetNS());
                byte[] datagram = wpis.Serialize();
                ClientSocket.Client.SendTo(datagram, EP);
            }
            foreach (Protocol wpis in niepotwierdzonePakiety)
            {
                niepotwierdzonePakiety.Remove(wpis);
            }
        }
            

    }
}