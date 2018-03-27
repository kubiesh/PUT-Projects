using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Net;
using server;

public class TcpTimeServer
{

    private const int portNum = 36666;

    public static int Main(String[] args)
    {
        Console.Title = "Server";
        Server serwer = new Server();
        serwer.RunServer();
        Console.ReadLine(); // When we press enter close everything
        serwer.CloseAllSockets();
        return 0;
    }

}
