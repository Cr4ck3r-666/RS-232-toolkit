#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

int BaudRate = 0;
int noDevices = 0;
string deviceName = "";
int deviceNumber = 0;
bool ifConnect = false;
string ports = "";

void configurePort(int fd, int baudRate) 
{
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);
    options.c_cflag = baudRate | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
}

// pierwsza zmiana w kodzie

void scanSerialPorts()
{
    int i;

    vector<string> ports;
    DIR* dir = opendir("/dev");
    if (dir != nullptr) 
    {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) 
        {
            if (strncmp(entry->d_name, "ttyUSB", 6) == 0 || strncmp(entry->d_name, "ttyACM", 6) == 0|| strncmp(entry->d_name, "ttyS0", 6) == 0)
            {
                string portName = "/dev/" + string(entry->d_name);
                ports.push_back(portName);
            }
        }
        closedir(dir);
    }
}

void setDevices() 
{
    scanSerialPorts();
    if (ports.empty()) 
    {
        cout << "RS232 devices are unplugged" << endl;
    } 
    else 
    {
        cout << "Available devices:" << endl;
        for (size_t i = 0; i < ports.size(); ++i) 
        {
            cout<< i + 1 << ". " << ports << endl;
        }
        cout<<"######################################"<<endl;
        cout<<"Set device (enter number):"<<endl;
        cin>> deviceNumber;
        if (deviceNumber > 0 && deviceNumber <= ports.size()) 
        {
            deviceName = ports;
        } 
        else 
        {
            cout << "Bad device" << endl;
        }
    }
}

void setBaudrate() 
{
    cout<< "Set baudrate:" << endl;
    cout<<"1. 50"<<endl;
    cout<<"2. 75"<<endl;
    cout<<"3. 110"<<endl;
    cout<<"4. 134"<<endl;
    cout<<"5. 150"<<endl;
    cout<<"6. 200"<<endl;
    cout<<"7. 300"<<endl;
    cout<<"8. 600"<<endl;
    cout<<"9. 1200"<<endl;
    cout<<"10. 1800"<<endl;
    cout<<"11. 2400"<<endl;
    cout<<"12. 4800"<<endl;
    cout<<"13. 9600"<<endl;
    cout<<"14. 19200"<<endl;
    cout<<"15. 38400"<<endl;
    cout<<"16. 57600"<<endl;
    cout<<"17. 115200"<<endl;
    cout<<"18. 230400"<<endl;
    cout<<"19. 460800"<<endl;
    cout<<"20. 921600"<<endl;
    int index;
    cin >> index;
    switch (index) 
    {
        case 1: 
        {
            BaudRate = 50; 
            break;            
        }

        case 2: 
        {
            BaudRate = 75; 
            break;  
        }
        case 3:
        {
            BaudRate = 110; 
            break;            
        }

        case 4: 
        {
            BaudRate = 134; 
            break;            
        }

        case 5:
        {
            BaudRate = 150; 
            break;            
        } 

        case 6: 
        {
            BaudRate = 200; 
            break;            
        }

        case 7:
        {
            BaudRate = 300; 
            break;            
        } 

        case 8: 
        {
            BaudRate = 600; 
            break;            
        }

        case 9:
        {
            BaudRate = 1200; 
            break;            
        } 

        case 10: 
        {
            BaudRate = 1800; 
            break;            
        }

        case 11: 
        {
            BaudRate = 2400; 
            break;            
        }

        case 12: 
        {
            BaudRate = 4800; 
            break;            
        }

        case 13: 
        {
            BaudRate = 9600; 
            break;            
        }

        case 14: 
        {
            BaudRate = 19200; 
            break;            
        }

        case 15: 
        {
            BaudRate = 38400; 
            break;            
        }

        case 16: 
        {
            BaudRate = 57600; 
            break;            
        }

        case 17: 
        {
            BaudRate = 115200; 
            break;            
        }

        case 18: 
        {
            BaudRate = 230400; 
            break;            
        }

        case 19: 
        {
            BaudRate = 460800; 
            break;            
        }

        case 20: 
        {
            BaudRate = 921600; 
            break;            
        }

        default: 
        {
            cout<<"Bad baudrate"<<endl; 
            break;            
        }

    }
}

int main() 
{
    menu:
    cout<<"######################################"<<endl;
    cout<<"##                                  ##"<<endl;
    cout<<"##       WELCOME TO RS232 TOOL      ##"<<endl;
    cout<<"##                                  ##"<<endl;
    cout<<"######################################"<<endl;
    cout<<"##                                  ##"<<endl;
    cout<<"## 1. Search device                 ##"<<endl;
    cout<<"## 2. Send to device                ##"<<endl;
    cout<<"## 3. Receive from device           ##"<<endl;
    cout<<"## 4. Settings                      ##"<<endl;
    cout<<"## 5. Exit                          ##"<<endl;
    cout<<"##                                  ##"<<endl;
    cout<<"######################################"<<endl;
    int i;
    cin >> i;
    switch (i) 
    {
        case 1: 
        {
            scanSerialPorts();
        
            if (ports.empty()) 
            {
                cout<< i <<"No serial ports found."<<endl;
            }    
            else 
            {
                cout<<"Found serial ports:"<<endl;
                for (const auto& port : ports) 
                {
                    cout<< i <<port<<endl;
                }
            }  
            goto menu;
            break;    
        }

        case 2:
        {
            if (deviceName.empty()) 
            {
                cout << "No device selected." << endl;
                break;
            }

            int fd = open(deviceName.c_str(), O_RDWR | O_NOCTTY);

            if (fd == -1) 
            {
                cout<<"Failed to open device."<<endl;
                break;
            }

            configurePort(fd, BaudRate);
            const char* data = "Hello, World!";
            write(fd, data, strlen(data));
            close(fd);
            goto menu;
            break;
        }

        case 3: 
        {
            if (deviceName.empty()) 
            {
                cout<<"No device selected."<<endl;
                break;
            }

            int fd = open(deviceName.c_str(), O_RDWR | O_NOCTTY);

            if (fd == -1) 
            {
                cout<<"Failed to open device."<<endl;
                break;
            }
            configurePort(fd, BaudRate);
            char buffer[256];
            int n = read(fd, buffer, sizeof(buffer));
            if (n > 0) 
            {
                cout<<"Received: "<<string(buffer, n)<<endl;
            }
            close(fd);
            goto menu;
            break;
        }

        case 4: 
        {
            setBaudrate();
            setDevices();
            goto menu;
            break;
        }

        case 5: 
        {
            return 0;
        }

        default: 
        {
            cout << "Bad option!" << endl;
        }
        
    }
    return 0;    
}