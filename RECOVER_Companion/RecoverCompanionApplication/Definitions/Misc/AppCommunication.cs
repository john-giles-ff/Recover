using NetMQ;
using NetMQ.Sockets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc
{
    class AppCommunication
    {
        public EventHandler<string> OnCommunicationRecieved;

        public void StartListening()
        {
            using (var responseSocket = new ResponseSocket("@tcp://localhost:9958"))
            {
                while (true)
                {
                    var message = responseSocket.ReceiveFrameString();
                    responseSocket.SendFrameEmpty();

                    OnCommunicationRecieved?.Invoke(this, message);
                }
            }
        }

        public static void SendMessage(string message)
        {
            using (var requestSocket = new RequestSocket(">tcp://localhost:9958"))
            {
                requestSocket.SendFrame(message);
                requestSocket.ReceiveFrameString();
            }
        }
    }
}
