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
        public Socket socket;
        private byte sessionID; //typ zmiennej moze byc zmieniony

        public Client(byte _sessionID, Socket _socket) {
            try {
                socket = _socket;
            }

            catch (ObjectDisposedException) // I cannot seem to avoid this (on exit when properly closing sockets)
            {
                return;
            }
            //Przypisanie identyfikatora sesji
            sessionID = _sessionID;


        }
        public byte getSessionID() {
            return sessionID;
        }

        public static byte randomID() {
            Random rnd = new Random();
            int randomInteger = rnd.Next(1, 63);
            byte randomID = Convert.ToByte(randomInteger);
            return randomID;
        }
    }
}
