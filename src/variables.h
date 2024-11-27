//sensor and timer
int light;
unsigned long startTime;
unsigned long currentTime;

//SD
const int size_1mb = 1048576;     //1 mb size 

//Wifi
const char* ssid = "ilfandkym";
const char* pw = "94263770";

//local time
//https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 8*3600;   //UTC +- n -> 3600*n
const int   daylightOffset_sec = 0; //No datelight saving