using System;

using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using static server.Protocol;

namespace server
{
    /*
      * ================================================================================================
      * KLASA SERVER
      * ================================================================================================
      */
    class Server
    {
        /*
        * ================================================================================================
        * DEKLARACJE POL
        * ================================================================================================
        */

        private static Socket _serverSocket; //Glowny socket
        private const int portNum = 36666; //Numer portu
        private const int _bufferSize = 14; //Wielkosc bufora rowna wielkosci pakietu
                                            // private int backlog = 100; //maksymalna ilosc polaczen

        private static byte[] _buffer; //Zmienna bedaca buforem
        private static List<Client> _clientSockets; //Tablica klientow wraz z socketami i ich identyfikatorami sesji
        public int LiczbaDoZgadniecia = 0;

        /*
        * ================================================================================================
        * DEKLARACJE POL - KONIEC
        * ================================================================================================
        */



        /*
        * ================================================================================================
        * KONSTRUKTOR
        * ================================================================================================
        */

        public Server()
        {
            //Tworzenie nowych obiektow
            _serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            _clientSockets = new List<Client>();
            _buffer = new byte[_bufferSize];
            _serverSocket.ReceiveTimeout = 2 * 60 * 1000; //Maksymalny czas czekania na odpowiedz klienta

        }
        /*
         * ================================================================================================
         * KONSTRUKTOR - KONIEC
         * ================================================================================================
         */
        public static int randomLiczba()
        {
            Random rnd = new Random();
            int randomInteger = rnd.Next(1, 20);
            return randomInteger;
        }

        /*
         * ================================================================================================
         * FUNKCJA RUNSERVER
         * GLOWNA FUNKCJA KLASY
         * ================================================================================================
         */
        public void RunServer()
        {
            SetupServer();
            Console.ReadLine(); // When we press enter close everything
            CloseAllSockets();
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
            _serverSocket.Bind(new IPEndPoint(IPAddress.Any, portNum)); //bind
            _serverSocket.Listen(64);
            _serverSocket.BeginAccept(AcceptCallBack, null);
            Console.WriteLine("Konfiguracja serwera zakonczona");
            Console.WriteLine("Oczekiwanie na polaczenia na porcie: " + portNum);
        }
        /*
         * ================================================================================================
         * KONFIGURACJA SERWERA - KONIEC
         * ================================================================================================
         */

        /*
        * ================================================================================================
        * AKCEPTACJA POLACZENIA KLIENTA
        * ================================================================================================
        */
        private static void AcceptCallBack(IAsyncResult AR)
        {

            Client clsocket;
            byte sessionID = wylosujID();
            try
            {
                clsocket = new Client(sessionID, _serverSocket.EndAccept(AR));
            }
            catch (ObjectDisposedException) // I cannot seem to avoid this (on exit when properly closing sockets)
            {
                return;
            }

            Console.WriteLine("[ID: " + clsocket.getSessionID() + "] Nawiazano polaczenie z klientem.");
            //Dodawanie klienta do listy
            _clientSockets.Add(clsocket);
            //Nawiazane polaczenie, wysylamy pakiet z flaga INIT
            SendInit(clsocket);
            //Zaczynamy odbierac od klienta dane
            clsocket.socket.BeginReceive(_buffer, 0, _bufferSize, SocketFlags.None, ReceiveCallBack, clsocket.socket);
            Console.WriteLine("[ID: " + clsocket.getSessionID() + "] Czekam na dane od klienta");
            _serverSocket.BeginAccept(AcceptCallBack, null);
        }
        /*
        * ================================================================================================
        * AKCEPTACJA POLACZENIA KLIENTA - KONIEC
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

            Socket s = (Socket)AR.AsyncState;
            //Z tablicy klientow pobieramy tego, do ktorego nalezy socket
            Client current = GetBySocket(s);
            int received;

            try
            {
                received = current.socket.EndReceive(AR);
            }
            catch (SocketException)
            {
                Console.WriteLine("[ID: " + current.getSessionID() + "] Klient zmuszony do rozlaczenia");
                // Don't shutdown because the socket may be disposed and its disconnected anyway.
                s.Close();
                _clientSockets.Remove(current);
                return;
            }

            //Sprawdzamy czy ilosc odebranych bajtow jest wieksza od 0, bo jak klient sie rozlacza to wysyla pusty pakiet,
            //a serwer tego nie lapie i wyrzuca wyjatek, wiec trzeba bylo to uwzglednic
            if (received > 0)
            {
                byte[] recBuf = new byte[received];
                Array.Copy(_buffer, recBuf, received);

                Protocol packet = new Protocol();
                //Przekazujemy recBuf, czyli tablice bajtow z naszymi danymi do funkcji deserializujacej
                packet.deserialize(recBuf);

                //Sprawdzamy czy ID sesji sie zgadza
                if (CheckValidateData(current, packet))
                {
                    //Jesli sie zgadza to wysylamy odpowiedz, ale najpierw przekazujemy do Action gdzie serwer obliczy
                    //zadanie klienta
                    byte[] data = Action(packet, current);
                    current.socket.Send(data);
                    Console.WriteLine("[ID: " + current.getSessionID() + "] Wyslano odpowiedz serwera.");
                }


                s.BeginReceive(_buffer, 0, _bufferSize, SocketFlags.None, ReceiveCallBack, s);
            }
            else
            {
                //Jesli otrzymalismy pusty pakiet to znaczy ze klient sie rozlaczyl(tylko w przypadku tego programu tak jest)
                //Zamykamy polaczenie z klientem, aby nie wyrzucilo wyjatku
                s.Shutdown(SocketShutdown.Both);
                s.Close();
                Console.WriteLine("[ID: " + current.getSessionID() + "] Klient rozlaczyl sie.");
                _clientSockets.Remove(current);
            }
        }
        /*
        * ================================================================================================
        * ODBIOR ODPOWIEDZI OD KLIENTA - KONIEC
        * ================================================================================================
        */

        /*
        * ================================================================================================
        * ZAMYKANIE SERWERA
        * FUNKCJA CLOSEALLSOCKETS
        * ================================================================================================
        */
        public void CloseAllSockets()
        {
            foreach (Client clsocket in _clientSockets)
            {
                clsocket.socket.Shutdown(SocketShutdown.Both);
                clsocket.socket.Close();
            }

            _serverSocket.Close();
        }

        /*
        * ================================================================================================
        * ZAMYKANIE SERWERA - KONIEC
        * ================================================================================================
        */


        /*
         * ================================================================================================
         * FUNKCJA CHECKCLIENTSESSIONID
         * SPRAWDZA CZY KLIENT O PODANYM ID ZNAJDUJE SIE NA LISCIE KLIENTOW POLACZONYCH
         * ZABEZPIECZENIE
         * ================================================================================================
         */
        private static bool checkClientSessionID(byte _sessionID)
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

        /*
        * ================================================================================================
        * FUNKCJA CHECKCLIENTSESSIONID - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA WYLOSUJID
        * ZLECA LOSOWANIE ID I SPRAWDZA CZY WYLOSOWANE ID ZOSTALO JUZ UZYTE
        * ================================================================================================
        */
        private static byte wylosujID()
        {
            byte wylosowany = Client.randomID();
            if (checkClientSessionID(wylosowany))
            {
                return wylosowany;
            }
            else
            {
                return wylosujID();
            }
        }

        /*
        * ================================================================================================
        * FUNKCJA WYLOSUJID - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA getSessionID
        * GETTER ID SESJI DLA KLIENTA
        * ================================================================================================
        */
        public static byte getSessionID(Socket socket)
        {
            foreach (Client cl in _clientSockets)
            {
                if (cl.socket == socket)
                {
                    return cl.getSessionID();
                }
            }
            //W razie niepowodzenia, wyjatki powinny zostac obsluzone wczesniej
            return 0;
        }

        /*
        * ================================================================================================
        * FUNKCJA getSessionID - KONIEC
        * ================================================================================================
        */


        /*
        * ================================================================================================
        * FUNKCJA GETBYSOCKET
        * ZWRACA OBIEKT KLIENTA DLA ZADANEGO SOCKETU
        * ================================================================================================
        */
        public static Client GetBySocket(Socket socket)
        {
            foreach (Client cl in _clientSockets)
            {
                if (cl.socket == socket)
                {
                    return cl;
                }
            }
            return null;

        }
        /*
        * ================================================================================================
        * FUNKCJA GETBYSOCKET - KONIEC
        * ================================================================================================
        */


        /*
         * ================================================================================================
         * FUNKCJA SENDINIT
         * ODPOWIADA ZA WYSLANIE PIERWSZEGO PAKIETU DO KLIENTA Z FLAGA INIT
         * NIE NAJBARDZIEJ OPTYMALNE ROZWIAZANIE
         * ================================================================================================
         */
        public static void SendInit(Client cl)
        {
            //Utworzenie pustego obiektu z pustymi polami danych
            Protocol packet = new Protocol();
            //Nadanie pola sesji
            packet.SetSessionID(cl.getSessionID());
            //Nadanie flagi INIT
            packet.SetInit(true);
            //Serializacja(Kodowanie binarne)
            byte[] data = packet.serialize();
            //Wyslanie pakietu do okreslonego klienta
            cl.socket.Send(data);
            Console.WriteLine("[ID: " + cl.getSessionID() + "] Wyslano pakiet INIT.");
        }
        /*
        * ================================================================================================
        * FUNKCJA SENDINIT - KONIEC
        * ================================================================================================
        */


        /*
         * ================================================================================================
         * FUNKCJA CHECKVALIDATEDATA
         * SPRAWDZA CZY IDENTYFIKATOR SESJI PRZYCHODZACEGO PAKIETU Z KONKRETNEGO SOCKETU POKRYWA SIE Z NADANYM
         * PRZEZ SERWER
         * ================================================================================================
         */
        public static bool CheckValidateData(Client cl, Protocol packet)
        {

            if (packet.GetSessionID() == cl.getSessionID())
            {
                Console.WriteLine("[ID: " + cl.getSessionID() + "] Identyfikator sesji odebranego pakietu jest zgodny.");
                return true;
            }
            else
            {
                Console.WriteLine("[ID: " + cl.getSessionID() + "] Identyfikator sesji nie zgadza sie z zapisanym. Prawdopodobnie proba przechwycenia danych.");
                CloseSocket(cl);
                return false;
            }
        }
        /*
        * ================================================================================================
        * FUNKCJA CHECKVALIDATEDATA - KONIEC
        * ================================================================================================
        */

        /*
         * ================================================================================================
         * FUNKCJA CLOSESOCKET
         * ZAMYKA POLACZENIE Z KLIENTEM JESLI ISTNIEJE POTRZEBA ZERWANIA POLACZENIA
         * ================================================================================================
         */
        public static void CloseSocket(Client cl)
        {
            cl.socket.Close();
            _clientSockets.Remove(cl);
        }
        /*
        * ================================================================================================
        * FUNKCJA CLOSESOCKET - KONIEC
        * ================================================================================================
        */

        /*
        * ================================================================================================
        * FUNKCJA ACTION
        * ODPOWIEDZIALNA ZA STWORZENIE PAKIETU BEDACEGO ODPOWIEDZIA NA ZADANIE KLIENTA
        * ================================================================================================
        */
        public static byte[] Action(Protocol packet, Client cl)
        {
            //W argumencie funkcji przekazujemy odebrany pakiet aby zrozumiec zadanie klienta
            Operacja _operation = packet.GetOperation();
            Status _state = Status.OK; //Domyslny status

            int wynik = 0;

            //W zaleznosci od operacji wynik bedzie inny
            switch (_operation)
            {
                case Operacja.DODAWANIE:

                    Int64 test = (Int64)packet.GetNumber1() + (Int64)packet.GetNumber2() + (Int64)packet.GetNumber3();
                    if (test > Int32.MaxValue)
                    {
                        _state = (Status.OVERRANGE);
                    }
                    else if (test < Int32.MinValue)
                    {
                        _state = (Status.UNDERRANGE);
                    }
                    else
                    {
                        wynik = packet.GetNumber1() + packet.GetNumber2() + packet.GetNumber3();
                    }
                    break;
                    ;
                case Operacja.ODEJMOWANIE:
                    Int64 testod = (Int64)packet.GetNumber1() - (Int64)packet.GetNumber2() - (Int64)packet.GetNumber3();
                    if (testod > Int32.MaxValue)
                    {
                        _state = (Status.OVERRANGE);
                    }
                    else if (testod < Int32.MinValue)
                    {
                        _state = (Status.UNDERRANGE);
                    }
                    else
                    {
                        wynik = packet.GetNumber1() - packet.GetNumber2() - packet.GetNumber3();
                    }
                    break;
                case Operacja.DZIELENIE:
                    if (packet.GetNumber2() == 0 || packet.GetNumber3() == 0)
                    {
                        _state = (Status.DIVISION0);
                    }
                    else
                    {
                        wynik = packet.GetNumber1() / packet.GetNumber2() / packet.GetNumber3();
                    }
                    break;
                case Operacja.SREDNIA:
                    wynik = (packet.GetNumber1() + packet.GetNumber2() + packet.GetNumber3()) / 3;
                    break;
            }
            //Tworzymy nowy obiekt, pusty pakiet do wyslania z niewypelnionymi polami
            Protocol sendpacket = new Protocol();
            //Wypelniamy pola
            sendpacket.SetStatus(_state);
            sendpacket.SetNumber1(wynik);
            sendpacket.SetOperation(_operation);
            sendpacket.SetSessionID(0);
            //Zwracamy zakodowany binarnie pakiet
            return sendpacket.serialize();

        }
        /*
        * ================================================================================================
        * FUNKCJA ACTION - KONIEC
        * ================================================================================================
        */  
    }
}
