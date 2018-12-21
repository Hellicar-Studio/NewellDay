using System;
using System.IO;
using System.Runtime.InteropServices;
using AxisMediaParserLib;

namespace Parser
{
    class Program
    {
        static BinaryWriter outFile;
        static object fileLock = new object ();
        private static string camIP;
        private static int recordingDuration;
        private static string dataPath;

        static void Main(string[] args)
        {
            // Look at our arguments
            for(int i = 0; i < args.Length; i++)
            {
                // Check if we are recieving the name of an argument
                if(args[i].Substring(0,2) == "--")
                {
                    // We got a real argument!
                    switch (args[i])
                    {
                        case "--data-path":
                            dataPath = args[++i];
                            break;
                        case "--cam-ip":
                            camIP = args[++i];
                            break;
                        case "--duration":
                            recordingDuration = Convert.ToInt32(args[++i]);
                            break;
                        default:
                            Console.WriteLine("Unknown Command: " + args[i] + " With argument: " + args[++i]);
                            break;
                    }
                }
                else
                {
                    // We did not get a real argument!
                    Console.WriteLine("Malformed Command: " + args[i] + ",\n Arguments must be preceded by -- and succeeded by their argument");
                }
            }

            // Create the AXIS Media Parser object and set connection properties
            AxisMediaParser parser = new AxisMediaParser();
            parser.MediaURL = "axmphttp://" + camIP + "/mjpg/1/video.mjpg";
            parser.MediaUsername = "root";
            parser.MediaPassword = "pass";
            // Register for OnVideoSample events
            parser.OnVideoSample += OnVideoSample;

            Console.WriteLine("Press Enter To Start");
            Console.ReadLine();

            try
            {
                using (FileStream outFileStream = new FileStream(dataPath, FileMode.Create)) using (outFile = new BinaryWriter(outFileStream))
                {
                    Console.WriteLine("Connecting to {0}", parser.MediaURL);
                    int cookieID;
                    int numberOfStreams;
                    object mediaBuffer;
                    parser.Connect(out cookieID, out numberOfStreams, out mediaBuffer);
                    // Write media type information to file (buffer is an array of bytes)
                    byte[] mediaTypeBuffer = (byte[])mediaBuffer;
                    outFile.Write(mediaTypeBuffer.Length);
                    outFile.Write(mediaTypeBuffer, 0, mediaTypeBuffer.Length);

                    // Start parser, OnVideoSample() will be called for each parsed frame
                    parser.Start();

                    // Sleep while OnVideoSample()
                    System.Threading.Thread.Sleep(recordingDuration * 1000);

                    // Stop the stream, the file C:\Axis\video.bin contains the 5 seconds video
                    parser.Stop();
                    Console.WriteLine("Stream stopped");
                    Console.ReadLine();
                }
            }
            catch (COMException e)
            {
                Console.WriteLine("Exception from URL: {0}, Error: {1}", parser.MediaURL, e.Message);
                Console.ReadLine();
            }
        }


        //Event hanler callbck for video samples buffers
        static void OnVideoSample(int cookieID, int sampleType, int sampleFlags, ulong startTime, ulong stopTime, object SampleArray)
        {
            byte[] bufferBytes = (byte[])SampleArray;
            Console.WriteLine("OnVideoSample - Received {0} bytes", bufferBytes.Length);
            lock (fileLock)
            {
                outFile.Write(sampleType);
                outFile.Write(sampleFlags);
                outFile.Write(startTime);
                outFile.Write(stopTime);
                outFile.Write(bufferBytes.Length);
                outFile.Write(bufferBytes, 0, bufferBytes.Length);
            }
        }
    }
}
