using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using static server.Protocol;
using System.Timers;
using System.Threading;

namespace server
{
    public class PakietNiepotwierdzony {
        public Client _client;
        public Protocol _packet;
        public int _count;
        public PakietNiepotwierdzony(Client client,Protocol packet)
        {
            _client = client;
            _packet = packet;
            _count = 0;
        }
    }
    class Server
    {
        private static Socket _serverSocket; //Glowny socket
        private const int portNum = 36666; //Numer portu
        private const int _bufferSize = 1024; //Wielkosc bufora rowna wielkosci pakietu
                                            // private int backlog = 100; //maksymalna ilosc polaczen
        private static byte[] _buffer; //Zmienna bedaca buforem
        private static byte[] _buffer2;
        private static List<Client> _clientSockets; //Tablica klientow wraz z socketami i ich identyfikatorami sesji
        private static List<PakietNiepotwierdzony> niepotwierdzonePakiety = new List<PakietNiepotwierdzony>();
        public static int liczbaDoZgadniecia = 0;
        public static int czasRozgrywki = 0;
        public static bool start = false;
        public static int maxClients = 3;
        public static int timeLeft = 0;
        private static System.Timers.Timer tim;
        private static System.Timers.Timer timRetransmisja;
        /*
         * ================================================================================================
         * KONSTRUKTOR
         * ================================================================================================
         */
        public Server()
        {
            //Tworzenie nowych obiektow
            _serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            _clientSockets = new List<Client>();
            _buffer = new byte[_bufferSize];
            _buffer2 = new byte[_bufferSize];
            liczbaDoZgadniecia = randomLiczba();
        }
        /*
         * ================================================================================================
         * KONSTRUKTOR - KONIEC
         * ================================================================================================
         */

     

        private static void ResetSerwera() {
            Console.WriteLine("Restart serwera...");
            liczbaDoZgadniecia = randomLiczba();
            _clientSockets.Clear();
            start = false;
            czasRozgrywki = 0;
            niepotwierdzonePakiety.Clear();
            EndPoint newClientEP = new IPEndPoint(IPAddress.Any, 0);
            _serverSocket.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref newClientEP, ReceiveCallBack, newClientEP);
            Console.WriteLine("Restart serwera zakonczony");
            Console.WriteLine("Oczekiwanie na polaczenia na porcie: " + portNum);
           
        }
        /*
         * 
     

       * ================================================================================================
       * FUNKCJA RUNSERVER
       * GLOWNA FUNKCJA KLASY
       * ================================================================================================
       */
        public void RunServer()
        {
            SetupServer();
            Console.ReadLine(); // When we press enter close everything
            //CloseAllSockets();
        }
        /*
        * ================================================================================================
        * FUNKCJA RUNSERVER - KONIEC
        * ================================================================================================
        */

        /*
         * ================================================================================================
         * KONFIGURACJA SERWERA
         * FUNKCJA SETUPSERVER
         * ================================================================================================
         */
        private static void SetupServer()
        {
            // Console.WriteLine(_serverSocket.AddressFamily..ToString());
            Console.WriteLine("Konfigurowanie serwera...");
            var EndPoint = new IPEndPoint(IPAddress.Any, portNum);
            _serverSocket.Bind(EndPoint); //bind
            EndPoint newClientEP = new IPEndPoint(IPAddress.Any, 0);
            _serverSocket.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref newClientEP, ReceiveCallBack, newClientEP);
            Console.WriteLine("Konfiguracja serwera zakonczona");
            Console.WriteLine("Oczekiwanie na polaczenia na porcie: " + portNum);
            timRetransmisja = new System.Timers.Timer(); //Tworzenie obiektu
            timRetransmisja.Interval = 5000; //Ustawienie przerywania na 1000ms (1s)
            timRetransmisja.Elapsed += RetransmisjaPakietow; //Przypisanie metody
            timRetransmisja.AutoReset = true;
            timRetransmisja.Start(); //Start timera
            timRetransmisja.Enabled = true;
        }
        /*
         * ================================================================================================
         * KONFIGURACJA SERWERA - KONIEC
         * ================================================================================================
         */

            


        /*
      * ================================================================================================
      * ODBIOR ODPOWIEDZI OD KLIENTA
      * FUNKCJA RECEIVECALLBACK
      * ================================================================================================
      */
        private static void ReceiveCallBack(IAsyncResult AR)
        {

            try
            {
               
                //EndPoint clientEP = (EndPoint)AR.AsyncState;
                //IPPacketInformation packetInfo;
                EndPoint clientEP = new IPEndPoint(IPAddress.Any, 0);
                //SocketFlags flags = SocketFlags.None;
                //Socket sock = (Socket)AR.AsyncState;
                int dataLen = 0;
                    
                Client cl = new Client(0, clientEP);
                try
                {
                    try
                    {
                        dataLen = _serverSocket.EndReceiveFrom(AR, ref clientEP);
                        EndPoint newClientEP = new IPEndPoint(IPAddress.Any, 0);

                        _serverSocket.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref newClientEP, ReceiveCallBack, newClientEP);

                    }
                    catch (SocketException) {
                        //EndPoint newClientEP = new IPEndPoint(IPAddress.Any, 0);
                        //_serverSocket.BeginReceiveFrom(_buffer, 0, _bufferSize, SocketFlags.None, ref newClientEP, ReceiveCallBack, newClientEP);
                        Console.WriteLine("Wyjatek 1");
                    }
                    catch (ObjectDisposedException) {
                        Console.WriteLine("Wyjatek 2");
                    }
                    //Console.WriteLine(Encoding.ASCII.GetString(_buffer, 0, dataLen));
                    Protocol receivedpacket = Odbior(dataLen);
                    cl = new Client(receivedpacket.GetID(), clientEP);
                    
                    if (receivedpacket.GetOP() != Operacja.Potwierdz)
                    {
                        try
                        {
                            SendAcknowledge(cl, receivedpacket.GetNS());//wyslanie potwierdzenia odbioru
                        }
                        catch (SocketException) { }


                        if (receivedpacket.GetOP() == Operacja.Nawiaz)
                        {
                            if (receivedpacket.GetDA() == 0)
                            {
                                if (_clientSockets.Count == maxClients || start == true)
                                {
                                    SendNawiaz(cl, Odpowiedz.JuzRozpoczeto, timeLeft);
                                    Console.WriteLine("Proba dolaczenia klienta po czasie.");
                                }
                                else
                                {
                                    if (!_clientSockets.Any(cli => cli.adreskoncowy.Equals(clientEP)))
                                    {
                                        int sessionID = wylosujID();
                                        //Console.WriteLine("Wylosowano ID: " + sessionID.ToString());
                                        cl = new Client(sessionID, clientEP);
                                        _clientSockets.Add(cl);
                                        Console.WriteLine(wyswietlID(receivedpacket) + "Dolaczyl nowy klient. Jego nowe ID to: " + cl.getSessionID().ToString());
                                        SendNawiaz(cl, Odpowiedz.OK, cl.getSessionID());

                                        if (_clientSockets.Count == maxClients && start == false)
                                        {
                                            Start();
                                        }
                                        else
                                        {
                                            Console.WriteLine("Czekamy na komplet graczy");
                                        }
                                    }
                                    else
                                    {
                                        Console.WriteLine(wyswietlID(receivedpacket) + "Istnieje klient w bazie z tym samym EndPoint.");

                                    }

                                }
                            }
                            
                            else
                            {
                                Console.WriteLine(wyswietlID(receivedpacket) + "Proba nawiazania polaczenia ze zlym polem danych");

                            }

                        }
                        else if (receivedpacket.GetOP() == Operacja.Rozlacz)
                        {
                            if (_clientSockets.Any(cli => cli.getSessionID().Equals(receivedpacket.GetID())))
                            {
                                Console.WriteLine(wyswietlID(receivedpacket) + "Rozlaczono.");
                                cl = ClientGetBySessionID(receivedpacket.GetID());
                                _clientSockets.Remove(cl);
                                niepotwierdzonePakiety.RemoveAll(wpis => wpis._client.getSessionID() == receivedpacket.GetID());
                            }
                            else
                            {
                                Console.WriteLine(wyswietlID(receivedpacket) + "Nie ma takiego klienta w bazie. Nie mozna rozlaczyc");
                            }
                        }
                        else if (receivedpacket.GetOP() == Operacja.Zgadnij)
                        {
                            if (CzyJestNaLiscie(receivedpacket.GetID()))
                            {
                                cl = ClientGetBySessionID(receivedpacket.GetID());
                                Console.WriteLine(wyswietlID(receivedpacket) + "Proba zgadniecia: "+ receivedpacket.GetDA());
                                Zgadnij(cl, receivedpacket.GetDA());
                            }
                            else
                            {
                                Console.WriteLine(wyswietlID(receivedpacket) + " Nie ma takiego klienta w bazie. Operacja Zgadnij");
                            }

                        }
                        //if (CzyJestNaLiscie(receivedpacket.GetID()))
                        //{ 
                        //    cl = ClientGetBySessionID(receivedpacket.GetID());
                        //    cl.ostNrSekw = receivedpacket.GetNS();
                        //}
                    }
                    else
                    {
                        
                        //JEZELI OTRZYMALISMY POTWIERDZENIE USUWAMY WPIS Z TABLICY NIEPOTWIERDZONYCH PAKIETOW
                        try
                        {
                            //Console.WriteLine(wyswietlID(receivedpacket) + "Potwierdzenie, NS:" + receivedpacket.GetNS());
                            //bool warunek= wpis.Item1.getSessionID() == receivedpacket.GetID() && wpis.Item2.GetNS() == receivedpacket.GetNS()
                            niepotwierdzonePakiety.RemoveAll(wpis => wpis._client.getSessionID() == receivedpacket.GetID() && wpis._packet.GetNS() == receivedpacket.GetNS());
                            
                        }
                        catch (Exception) { }

                        
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
               
            }
            catch (ObjectDisposedException)
            {
            }
            catch (SocketException)
            {
            }
            finally
            {

              
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
        private static String wyswietlID(Protocol packet)
        {
            String line = "[ID: " + packet.GetID().ToString() + "] ";
            return line;
        }


        /*
        * ================================================================================================
        * ODBIOR ODPOWIEDZI OD KLIENTA - KONIEC
        * ================================================================================================
        */
        public static void SendPacket(Client cl, Protocol packet) {
            //packet.SetID(cl.getSessionID());

            if (packet.GetOP() != Operacja.Potwierdz) {
                packet.SetNS(cl.ostNrSekw);
                
                niepotwierdzonePakiety.Add(new PakietNiepotwierdzony(cl,packet));
                cl.ostNrSekw++;
            }
            packet.SetID(cl.getSessionID());
            byte[] datagram = packet.Serialize();
           
            _serverSocket.SendTo(datagram, cl.adreskoncowy);
            cl.iloscniepotwierdzonych++;
            Thread.Sleep(15);
        }
       
        public static void SendAcknowledge(Client cl, int nrSekw) {
            Protocol packet = new Protocol();  
            packet.SetAckowlegement(nrSekw);
            SendPacket(cl, packet);
        }
        public static void Zgadnij(Client cl, int liczba)
        {
            if (liczba == liczbaDoZgadniecia)
            {
                foreach (Client cli in _clientSockets)
                {
                    if (cli.getSessionID() == cl.getSessionID())
                    {
                        SendCzyOdgadnieta(cli, Odpowiedz.Tak, 1);
                    }
                    else
                    {
                        SendCzyOdgadnieta(cli, Odpowiedz.Tak, 0);
                    }
                }
                Stop(Odpowiedz.Koniec);
            }
            else if ( liczba < liczbaDoZgadniecia )
            {
                SendCzyOdgadnieta(cl, Odpowiedz.ZaMala, 0);
            } else if (liczba > liczbaDoZgadniecia)
            {
                SendCzyOdgadnieta(cl, Odpowiedz.ZaDuza, 0);
            }
        }
        public static void SendCzyOdgadnieta(Client cl,Odpowiedz OD,int DA) {
            Protocol packet = new Protocol();
            packet.SetOP(Operacja.CzyOdgadnieta);
            packet.SetOD(OD);
            packet.SetDA(DA);
            packet.SetID(cl.getSessionID());
            SendPacket(cl, packet);
        }
        public static void SendNawiaz(Client cl, Odpowiedz OD, int DA)
        {
            Protocol packet = new Protocol();
            packet.SetOP(Operacja.Nawiaz);
            packet.SetOD(OD);
            packet.SetDA(DA);
            SendPacket(cl, packet);
        }
        public static void SendStop(Client cl, Odpowiedz odp)
        {
            Protocol packet = new Protocol();
            packet.SetOP(Operacja.Stop);
            packet.SetOD(odp);
            packet.SetDA(liczbaDoZgadniecia);
            //byte[] datagram = packet.Serialize();
            ////EndPoint broadcast = new IPEndPoint(IPAddress.Any, portNum);
            //_serverSocket.SendTo(datagram, cl.adreskoncowy);
            SendPacket(cl, packet);
        }
        public static void Stop(Odpowiedz odp) {

            foreach (Client cli in _clientSockets)
            {
                SendStop(cli, odp);

            }
            
            timRetransmisja.Stop();
            timRetransmisja.Enabled = false;
            tim.Stop();
            tim.Enabled = false;
            Thread.Sleep(100);
            _clientSockets.Clear();
            ResetSerwera();
        }
        public static void SendStart(Client cl)
        {
            Protocol packet = new Protocol();
            packet.SetOP(Operacja.Start);
            packet.SetOD(Odpowiedz.OK);
            packet.SetDA(maxClients);
            SendPacket(cl, packet);
        }
        public static void SendCzasDoKonca(Client cl,int czasDoKonca)
        {
            Protocol packet = new Protocol();
            packet.SetID(cl.getSessionID());
            packet.SetOP(Operacja.CzasDoKonca);
            packet.SetOD(Odpowiedz.OK);
            packet.SetDA(czasDoKonca);
            //EndPoint broadcast = new IPEndPoint(IPAddress.Any, portNum);
            //byte[] datagram = packet.Serialize();
            //_serverSocket.SendTo(datagram, cl.adreskoncowy);
            SendPacket(cl, packet);
        }
        public static void CzasDoKonca(int czasDoKonca)
        {
            foreach (Client cli in _clientSockets)
            {
                SendCzasDoKonca(cli, czasDoKonca);
            }
        }
        public static void Start()
        {
           
            Console.WriteLine("Mamy komplet graczy. Zaczynamy gre!");
            Console.WriteLine("Liczba do zgadniecia to: " + liczbaDoZgadniecia);

            //Protocol packet = new Protocol();
            //packet.SetOP(Operacja.Start);
            //packet.SetOD(Odpowiedz.OK);
            //packet.SetDA(maxClients);
            //EndPoint broadcast = new IPEndPoint(IPAddress.Any, portNum);
            //byte[] datagram = packet.Serialize();
            //_serverSocket.SendTo(datagram, broadcast);
            foreach (Client cli in _clientSockets)
            {
                SendStart(cli);
            }
            setCzasRozgrywki();
            Console.WriteLine("Czas na odgadniecie to: " + czasRozgrywki+ " sekund");
            CzasDoKonca(czasRozgrywki);
            
            tim = new System.Timers.Timer(); //Tworzenie obiektu
            tim.Interval = 10000; //Ustawienie przerywania na 10000ms (10s)
            tim.Elapsed += OdliczAkcja; //Przypisanie metody
            tim.AutoReset = true;
            tim.Start(); //Start timera
            tim.Enabled = true;
            start = true;

        }
        private static void OdliczAkcja(Object source, ElapsedEventArgs e)
        {
            if (start == true)
            {
                if (timeLeft-10<0)
                {
                    timeLeft = 0;
                }
                else
                {
                    timeLeft -= 10;
                }
                
                if (timeLeft > 0)
                {
                    CzasDoKonca(timeLeft);

                }
                else
                {
                    Console.WriteLine("Koniec czasu");
                    CzasDoKonca(0);
                    tim.Enabled = false;
                    tim.Stop();
                    Stop(Odpowiedz.Koniec);
                }
            }
            else
            {
                tim.Enabled = false;
                tim.Stop();
            }
        }
        private static void RetransmisjaPakietow(Object source, ElapsedEventArgs e)
        {
            try
            {
                foreach (PakietNiepotwierdzony wpis in niepotwierdzonePakiety)
                {
                    Console.WriteLine("Retransmisja pakietu: ID-" + wpis._client.getSessionID() + ", NS-" + wpis._packet.GetNS());
                    byte[] datagram = wpis._packet.Serialize();
                    
                    try 
                    {
                        if (wpis._count < 3)
                        {
                            _serverSocket.SendTo(datagram, wpis._client.adreskoncowy);
                            Thread.Sleep(15);
                        }
                        else
                        {
                            try
                            {
                                niepotwierdzonePakiety.RemoveAll(rekord => rekord._client.getSessionID() == wpis._client.getSessionID());
                            }
                            catch (Exception) { };
                        }
                    }
                    catch (SocketException)
                    {
                        niepotwierdzonePakiety.RemoveAll(rekord => rekord._client.getSessionID() == wpis._client.getSessionID());
                    }
                    catch (Exception) {
                        niepotwierdzonePakiety.RemoveAll(rekord => rekord._client.getSessionID() == wpis._client.getSessionID());
                    }
                    wpis._count++;
                }
                niepotwierdzonePakiety.RemoveAll(wpis => wpis._count.Equals(3));
            }
            catch (Exception)
            {

            }
        }

        private static int wylosujID()
        {
            int wylosowany = Client.randomID();
            if (checkClientSessionID(wylosowany))
            {
                return wylosowany;
            }
            else
            {
                return wylosujID();
            }
        }
        private static bool checkClientSessionID(int _sessionID)
        {

            foreach (Client cl in _clientSockets)
            {
                if (cl.getSessionID() == _sessionID)
                {
                    return false;
                }
            }
            return true;
        }
        public static Client ClientGetBySessionID(int SessionID)
        {
            foreach (Client cl in _clientSockets)
            {
                if (cl.getSessionID()== SessionID)
                {
                    return cl;
                }
            }
            return null;

        }
        public static Client ClientGetByEndPoint(EndPoint ep)
        {
            foreach (Client cl in _clientSockets)
            {
                if (cl.adreskoncowy.Equals(ep))
                {
                    return cl;
                }
            }
            Console.WriteLine("Nie znaleziono po EndPoint");
            return new Client(0,ep);

        }
        public static void setCzasRozgrywki()
        {
            int sumaID = 0;
            int wyliczonyczas = 0;
            if (_clientSockets.Count == maxClients)
            {
                foreach (Client cl in _clientSockets)
                {
                    sumaID += cl.getSessionID();
                }
                wyliczonyczas = ((sumaID * 99) % 100) + 30;
                czasRozgrywki = wyliczonyczas;
                timeLeft = wyliczonyczas;
            }
            else
            {
                Console.WriteLine("Nie powinno sie ustawiac czasu rozgrywki jesli nie ma wszystkich klientow w bazie.");
            }


        }
        public static bool CzyJestNaLiscie(int IDsesji)
        {
            if (_clientSockets.Any(cli => cli.getSessionID().Equals(IDsesji)))
            {
                return true;
            }
            return false;
        }
        public static int randomLiczba()
        {
            Random rnd = new Random();
            int randomInteger = rnd.Next(1, 2048);
            return randomInteger;
        }
    }
   
}
