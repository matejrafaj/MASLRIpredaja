using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ZynqCamApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        [DllImport("chacha20.dll", EntryPoint = "chacha20_use")]
        public static extern void chacha20_use(byte[] bytes, UInt64 n_bytes, byte[] key, byte[] nonce, UInt64 counter);
        [DllImport("decoder.dll", EntryPoint = "decode")]
        public static extern void decode(byte[] buffer, byte[] output);

        public MainWindow()
        {
            InitializeComponent();
        }

        private void startButton_Click(object sender, RoutedEventArgs e)
        {

            int width = 640;
            int height = 480;


            // Posalji kameri

            TcpClient client = new TcpClient("192.168.1.10", 7);
            NetworkStream stream = client.GetStream();

            string autoContrastText = autoContrastState.SelectedValue.ToString();

            Byte[] data = { 0x41 , 0x00, 0x00, 0x00, 0x00 };

            if((bool)sendTestBar.IsChecked)
            {
                data[1] = 0x01;
            }

            if (autoContrastText.Equals("Soft Post Contrast+"))
            {
                data[2] = 0x02;
            }
            if (autoContrastText.Equals("Hard Post Contrast+"))
            {
                data[2] = 0x01;
            }

            if((bool)vFlip.IsChecked)
            {
                data[3] |= 0x01;
            }
            if ((bool)hMirror.IsChecked)
            {
                data[3] |= 0x02;
            }
            if((bool)compress.IsChecked)
            {
                data[4] = 0x01;
            }

            stream.Write(data, 0, data.Length);

            // Primi sliku od kamere

            data = new Byte[width * height + width * height / 2];

            stream.Read(data, 0, 4);
            int totalLength = BitConverter.ToInt32(data, 0);
            int length = totalLength;

            Byte[] compressedData = new byte[totalLength];

            int bytesRead;
            while (length > 0)
            {
                bytesRead = stream.Read(compressedData, totalLength - length, compressedData.Length);
                length -= bytesRead;
            }

            byte[] key = { 253, 232, 117, 230, 4, 234, 79, 143, 10, 13, 82, 236, 43, 23, 155, 240, 76, 149, 190, 222, 86, 180, 235, 136, 141, 79, 82, 232, 152, 100, 154, 88 };
            byte[] nonce = { 219, 217, 216, 81, 110, 153, 222, 222, 85, 76, 93, 118 };

            chacha20_use(compressedData, (ulong)totalLength, key, nonce, 0);

            // Dekodiraj
            if((bool)compress.IsChecked)
            {
                decode(compressedData, data);
            }
            else
            {
                data = compressedData;
            }
            
            Byte[] colors = new Byte[width * height * 3];

            int c = 0;
            for(int j = 0; j < height; j++)
            {
                for(int i = 0; i < width; i++)
                {
                    int y = data[j * width + i];
                    int u = data[640 * 480 + ((j/2) * (width/2)) + (i / 2)];
                    int v = data[640 * 480 + 320 * 240 + ((j / 2) * (width / 2)) + (i / 2)];

                    colors[c] = (byte)Math.Min(Math.Max(1.164 * (y - 16) + 1.596 * (v - 128), 0), 255);
                    colors[c + 1] = (byte)Math.Min(Math.Max(1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128), 0), 255);
                    colors[c + 2] = (byte)Math.Min(Math.Max(1.164 * (y - 16) + 2.018 * (u - 128), 0), 255);

                    c += 3;
                }
            }

            WriteableBitmap wbm = new WriteableBitmap(width, height, 96, 96, PixelFormats.Rgb24, null);
            wbm.WritePixels(new Int32Rect(0, 0, width, height), colors, 3 * width, 0);
            campic.Source = wbm;

        }

    }
}
