using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;

namespace server
{
    public class Client
    {
        public EndPoint adreskoncowy;
        private int sessionID; //typ zmiennej moze byc zmieniony
        public int ostNrSekw;
        public bool rozlaczony = false;
        public int iloscniepotwierdzonych =0;
        public Client(int _sessionID, EndPoint ep)
        {
            try
            {
                this.adreskoncowy = ep;
            }

            catch (ObjectDisposedException) // I cannot seem to avoid this (on exit when properly closing sockets)
            {
                return;
            }
            //Przypisanie identyfikatora sesji
            sessionID = _sessionID;
            ostNrSekw = server.Server.randomLiczba();

        }
        public int getSessionID()
        {
            return sessionID;
        }



        public static int randomID()
        {
            Random rnd = new Random();
            int randomInteger = rnd.Next(1, 1000);
            return randomInteger;
        }
    }
}
