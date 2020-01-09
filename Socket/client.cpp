#include "mbed.h"

// Network interface
NetworkInterface *net;

//serial
Serial pc (USBTX, USBRX,NULL,115200);
Serial device(D1,D0,NULL,115200);


// Socket demo
int main() {
    int remaining;
    int rcount;
    char *p;
    char *buffer = new char[256];
    nsapi_size_or_error_t result;

    // Bring up the ethernet interface
    printf("Mbed OS Socket example\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    net = NetworkInterface::get_default_instance();

    if (!net) {
        printf("Error! No network inteface found.\n");
        return 0;
    }

    result = net->connect();
    if (result != 0) {
        printf("Error! net->connect() returned: %d\n", result);
        return result;
    }

    // Show the network address
    const char *ip = net->get_ip_address();
    const char *netmask = net->get_netmask();
    const char *gateway = net->get_gateway();
    printf("IP address: %s\n", ip ? ip : "None");
    printf("Netmask: %s\n", netmask ? netmask : "None");
    printf("Gateway: %s\n", gateway ? gateway : "None");

    // Open a socket on the network interface, and create a TCP connection to ifconfig.io
    TCPSocket socket;

    result = socket.open(net);
    if (result != 0) {
        printf("Error! socket.open() returned: %d\n", result);
    }

    result = socket.connect("192.168.43.61", 65431);
    if (result != 0) {
        printf("Error! socket.connect() returned: %d\n", result);
        goto DISCONNECT;
    }

   char msg;
   char sbuffer[30];
   //char readNum[30];
   //char sendNum[30];
   //bool isNumber; isNumber=0;
   //bool isValid; isValid=1;
   bool concat; concat = false;

   while (true) {
         
        msg = device.getc();
        //pc.printf("%c\n",msg);
        //isValid=1;
            
       switch(msg){
            case 'a':
                if(concat) { 
                    strcat(sbuffer,", no activity");
                    concat = false;
                }
                else strcpy(sbuffer,"no activity");
                
                break;
            case 'b':
                if(concat) { 
                    strcat(sbuffer,", stationary");
                    concat = false;
                }
                else strcpy(sbuffer,"stationary");
                 break;
            case 'c':
                if(concat) { 
                    strcat(sbuffer,", standing");
                    concat = false;
                }
                else strcpy(sbuffer,"standing");
                break;
            case 'd':
                if(concat) { 
                    strcat(sbuffer,", sitting");
                    concat = false;
                }
                else strcpy(sbuffer,"sitting");
                break;
            case 'e':
                if(concat) { 
                    strcat(sbuffer,", lying");
                    concat = false;
                }
                else strcpy(sbuffer,"lying");
                break;
            case 'f':
                if(concat) { 
                    strcat(sbuffer,", walking");
                    concat = false;
                }
                else strcpy(sbuffer,"walking");
                break;
            case 'g':
                if(concat) { 
                    strcat(sbuffer,", fast walking");
                    concat = false;
                }
                else strcpy(sbuffer,"fast walking");
                break;
            case 'h':
                if(concat) { 
                    strcat(sbuffer,", jogging");
                    concat = false;
                }
                else strcpy(sbuffer,"jogging");
                break;
            case 'i':
                if(concat) { 
                    strcat(sbuffer,", biking");
                    concat = false;
                }
                else strcpy(sbuffer,"biking");
                break;
            case 'k':
                strcpy(sbuffer,"unknown desk");
                break;
            case 'l':
                strcpy(sbuffer,"sitting desk");
                break;
            case 'm':
                strcpy(sbuffer,"standing desk");
                break;
            case 'n':
                strcpy(sbuffer,"no sleeping");
                concat = true;
                break;
            case 'o':
                strcpy(sbuffer,"sleeping");
                break;
            case 'p':
                strcpy(sbuffer,"fall down");
                break;
            case 'q':
                strcpy(sbuffer,"turn over");
                break;

            default:
                break;
       }
       //if concat = true, wait for concat complete to send sbuffer
       if(!concat){
            nsapi_size_t size = strlen(sbuffer);
            // Loop until whole request sent
            result=0;
            if(0<(result = socket.send(sbuffer, size))){
                printf("sent %d [%s]\n", result, sbuffer);
            }
            if (result < 0) {
                printf("Error! socket.send() returned: %d\n", result);
                goto DISCONNECT;
            } 
            // Receieve an HTTP response and print out the response line
            remaining = 256;
            result=0;
            if( 0 < (result = socket.recv(buffer, remaining))) {
                buffer[result] = '\0';
                printf("recv %d [%s]\n", result, buffer);      
            }
            if (result < 0) {
                printf("Error! socket.recv() returned: %d\n", result);
                goto DISCONNECT;
            }   
       }  
    }               

    delete[] buffer;

DISCONNECT:
    // Close the socket to return its memory and bring down the network interface
    socket.close();
    // Bring down the ethernet interface
    net->disconnect();
    printf("Done\n");
}
