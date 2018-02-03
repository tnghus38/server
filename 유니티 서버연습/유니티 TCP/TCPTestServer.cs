using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;
public class TCPTestServer : MonoBehaviour
{
    public string Url;
    public int port;
    public UnityEngine.UI.InputField msg_text;
    #region private members 	
    /// <summary> 	
    /// TCPListener to listen for incomming TCP connection 	
    /// requests. 	
    /// </summary> 	
    /// 
    private TcpListener tcpListener;
    /// <summary> 
    /// Background thread for TcpServer workload. 	
    /// </summary> 	
    private Thread tcpListenerThread;
    /// <summary> 	
    /// Create handle to connected tcp client. 	
    /// </summary> 	
    private TcpClient connectedTcpClient;
    #endregion
   
    TcpClient[] client_array  = new TcpClient[5];
    int index=0;
    // Use this for initialization
    void Start()
    {
        // Start TcpServer background thread 		
        tcpListenerThread = new Thread(new ThreadStart(ListenForIncommingRequests));
        tcpListenerThread.IsBackground = true;
        tcpListenerThread.Start();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Return))
        {
            SendMessage();
        }
    }

    private void OnDestroy()
    {
        tcpListener.Stop();
        tcpListenerThread.Abort();
    }
    private void childSocketRecvThread(System.Object obj)
    {
        TcpClient a = (TcpClient)obj;
        Byte[] bytes = new Byte[1024];
        // Get a stream object for reading 					
        NetworkStream stream = a.GetStream();
        
        
          
            int length;
        // Read incomming stream into byte arrary. 						
        while (stream.CanRead)
        {
            length = stream.Read(bytes, 0, bytes.Length);
            var incommingData = new byte[length];
            Array.Copy(bytes, 0, incommingData, 0, length);
            // Convert byte array to string message. 							
            string clientMessage = Encoding.UTF8.GetString(incommingData);
            text.AddText(clientMessage);
            for (int j = 0; j < index; j++)
            {
                NetworkStream stream2 = client_array[j].GetStream();
                stream2.Write(incommingData, 0, incommingData.Length);
            }




        }
        
    }
    /// <summary> 	
    /// Runs in background TcpServerThread; Handles incomming TcpClient requests 	
    /// </summary> 	
    private void ListenForIncommingRequests()
    {
        try
        {
            // Create listener on localhost port 8052. 			
            tcpListener = new TcpListener(IPAddress.Parse(Url), port);
            tcpListener.Start();
            text.AddText("Server is listening \n");
   
           
            while (true)
            {

                TcpClient a;
                
                a = tcpListener.AcceptTcpClient();
                    client_array[index]=a;
                    index++;
                    var childSocketThread = new Thread(new ParameterizedThreadStart(childSocketRecvThread));
                    childSocketThread.Start(a);
                 

                
                 
            }
        }
        catch (SocketException socketException)
        {
            Debug.Log("SocketException " + socketException.ToString());
        }
    }
    
    /// <summary> 	
    /// Send message to client using socket connection. 	
    /// </summary> 	
    private void SendMessage()
    {
        if (client_array[0] == null)
        {
            return;
        }

        try
        {
            // Get a stream object for writing. 			
            for (int i = 0; i < index; i++)
            {
                NetworkStream stream = client_array[i].GetStream();
                if (stream.CanWrite)
                {

                    string ssss = "server:" + msg_text.text;
                    // Convert string message to byte array.                 
                    byte[] serverMessageAsByteArray = Encoding.UTF8.GetBytes(ssss);
                    // Write byte array to socketConnection stream.               
                    stream.Write(serverMessageAsByteArray, 0, serverMessageAsByteArray.Length);
                    text.AddText(ssss);
                }
            }
        }
        catch (SocketException socketException)
        {
            Debug.Log("Socket exception: " + socketException);
        }
    }
}