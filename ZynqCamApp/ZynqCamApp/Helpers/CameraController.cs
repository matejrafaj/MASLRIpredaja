using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ZynqCamApp.Helpers
{
    public static class CameraController
    {

        public static byte[] TakePicture(string IPAddress, short port, byte[] options)
        {
            byte[] picture;

            TcpClient client = new TcpClient(IPAddress, port);
            NetworkStream stream = client.GetStream();

            stream.Write(options, 0, options.Length);

            int size;
            byte[] sizeBytes = new byte[sizeof(int)];

            stream.Read(sizeBytes, 0, sizeof(int));

            size = BitConverter.ToInt32(sizeBytes, 0);

            picture = new byte[size];

            stream.Read(picture, 0, size);

            stream.Close();
            client.Close();

            return picture;
        }

    }
}
