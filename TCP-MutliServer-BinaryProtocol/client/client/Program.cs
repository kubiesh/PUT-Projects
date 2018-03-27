using System;
using System.Text;
using System.Net.Sockets;
using System.Net;
using server;
using static server.Protocol;


//TUTAJ NIE MA FINEZJI, WIEC DUZO NIE KOMENTUJE

namespace MultiClient
{
    class Program
    {

        /*
         * ================================================================================================
         * DEKLARACJE
         * ================================================================================================
         */
        private static readonly Socket ClientSocket = new Socket
            (AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        private const int PORT = 36666;
        private static byte sessionID = 0;
        private static int _bufferSize = 14;
        private static byte[] _buffer;
        private static int number1;
        private static int number2;
        private static int number3;

        /*
         * ================================================================================================
         * DEKLARACJE - KONIEC
         * ================================================================================================
         */



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
            ClientSocket.ReceiveTimeout=100;
            ConnectToServer();
            RequestLoop();
            Exit();
        }
        /*
        * ================================================================================================
        * FUNKCJA MAIN - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA CONNECTTOSERVER
        * LACZENIE SIE Z SERWEREM, DOPOKI SIE NIE POLACZY PROGRAM NIE PRZEJDZIE DALEJ
        * ================================================================================================
        */
        private static void ConnectToServer()
        {
            int attempts = 0;

            while (!ClientSocket.Connected)
            {
                try
                {
                    attempts++;
                    Console.WriteLine("Proba polaczenia nr: " + attempts);
                    // Change IPAddress.Loopback to a remote IP to connect to a remote host
                    IPAddress adrr = IPAddress.Parse("127.0.0.1");
                    ClientSocket.Connect(adrr, PORT);
                    Console.WriteLine(ClientSocket.AddressFamily.ToString());
                }
                catch (SocketException)
                {
                    Console.Clear();
                }
            }

            Console.Clear();
            Console.WriteLine("Polaczono z serwerem.");
            Console.WriteLine("Oczekiwanie na pakiet z identyfikatorem sesji.");
        }
        /*
        * ================================================================================================
        * FUNKCJA CONNECTTOSERVER - KONIEC FUNKCJI
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA REQUESTLOOP
        * W KOLKO POWTARZA SIE ODBIERANIE ODPOWIEDZI I WYSYLANIE ZADAN
        * JAKO PIERWSI ODBIERAMY ODPOWIEDZ, DLATEGO NAJPIERW JEST RECEIVERESPONSE,
        * ALE PODEJRZEWAM, ZE I TAK TE FUNKCJE SIECIOWE DZIALAJA JAKO WATKI W TLE
        * ================================================================================================
        */
        private static void RequestLoop()
        {
            while (true)
            {
                ReceiveResponse();
                SendRequest();
                
            }
        }
        /*
        * ================================================================================================
        * FUNKCJA REQUESTLOOP - KONIEC
        * ================================================================================================
        */

        /// <summary>
        /// Close socket and exit program.
        /// </summary>
        private static void Exit()
        {
            ClientSocket.Shutdown(SocketShutdown.Both);
            ClientSocket.Close();
            Environment.Exit(0);
        }




        /*
        * ================================================================================================
        * FUNKCJA SENDREQUEST
        * WYSYLANIE ZADANIA DO SERWERA LUB ROZLACZENIE
        * ================================================================================================
        */
        private static void SendRequest()
        {
            byte _wyboroperacji=new byte();
            Operacja _operative = Operacja.ODEJMOWANIE;
            Console.Write("Wybierz operacje ktora chcesz wykonac:\n1 - Odejmowanie\n2 - Dzielenie\n3 - Dodawanie\n4 - Srednia\n5 - Rozlacz sie\n");

            _wyboroperacji = Convert.ToByte(Console.ReadLine());
            switch (_wyboroperacji) {
                case 1: _operative = Operacja.ODEJMOWANIE;
                    break;
                case 2: _operative = Operacja.DZIELENIE;
                    break;
                case 3: _operative = Operacja.DODAWANIE;
                    break;
                case 4: _operative = Operacja.SREDNIA;
                    break;
                case 5:
                    Exit();
                    break;
            }
            Console.WriteLine("Wprowadz liczbe nr 1:");
            number1 = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("Wprowadz liczbe nr 2:");
            number2 = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("Wprowadz liczbe nr 3:");
            number3 = Convert.ToInt32(Console.ReadLine());

            Protocol packet = new Protocol();
            packet.SetSessionID(sessionID);
            packet.SetNumber1(number1);
            packet.SetNumber2(number2);
            packet.SetNumber3(number3);
            packet.SetOperation(_operative);
            _buffer = packet.serialize();
            ClientSocket.Send(_buffer, 0, _bufferSize, SocketFlags.None);

        }
        /*
        * ================================================================================================
        * FUNKCJA SENDREQUEST - KONIEC
        * ================================================================================================
        */

        /*
        * ================================================================================================
        * FUNKCJA RECEIVERESPONSE
        * ODBIERANIE ODPOWIEDZI OD SERWERA
        * ================================================================================================
        */
        private static void ReceiveResponse()
        {
            var buffer = new byte[_bufferSize];
            int received = ClientSocket.Receive(buffer, SocketFlags.None);
            if (received == 0) return;
            var data = new byte[received];
            Array.Copy(buffer, data, received);
            //data przechowuje pakiet
            Protocol packet = new Protocol();
            packet.deserialize(data);
            if (packet.GetInit() == true)
            {
                sessionID = packet.GetSessionID();
                Console.WriteLine("Otrzymano pakiet INIT, Twoje ID sesji to: " + sessionID);
            }
            else {
                Console.WriteLine("Otrzymano odpowiedz z serwera.");
                if (packet.GetStatus()!=Status.OK)
                {
                    Console.WriteLine("Blad!");
                }
                Console.WriteLine("Status: " + packet.GetStatus().ToString());
                Console.WriteLine("Operacja: " + packet.GetOperation().ToString());
                if (packet.GetStatus() == Status.OK)
                {
                    Console.WriteLine("Wynik: " + packet.GetNumber1());
                }
            }
        }
        /*
        * ================================================================================================
        * FUNKCJA RECEIVERESPONSE - KONIECmmmmmmmmmmm
        * ================================================================================================
        */

    }
}