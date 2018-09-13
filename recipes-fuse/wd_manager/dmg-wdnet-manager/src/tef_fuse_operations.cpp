//#include "projectinclude.h"

#include "tef_fuse_operations.h"

#define PATH_WD_START           "/wd_start"
#define PATH_WD_STOP            "/wd_stop"
#define PATH_WD_ALIVE           "/wd_alive"
#define PATH_WD_REMTIME         "/wd_countdown"
#define PATH_WD_VERSION         "/wd_version"



#define LEN_INFO  1024

#define LAN_OFF   "/usr/bin/lan_off.sh"
#define WIFI_OFF  "/usr/bin/wifi_off.sh"




pthread_mutex_t TefFuseOperations::m_mutex_wd_Alive;
pthread_mutex_t TefFuseOperations::m_mutex_wd_status;
bool            TefFuseOperations::m_wd_Active = false;
bool            TefFuseOperations::m_wd_Alive = false;
bool            TefFuseOperations::m_wd_Loop = false;

uint32_t TefFuseOperations::uiwTimeEth  = DEFAULT_TIME;
uint32_t TefFuseOperations::uiwTimeWifi = DEFAULT_TIME;
uint32_t TefFuseOperations::uiwSecTime  = SECOND_IN_MIN;

uint32_t TefFuseOperations::uiwTimeEth_ResetValue  = DEFAULT_TIME;
uint32_t TefFuseOperations::uiwTimeWifi_ResetValue = DEFAULT_TIME;


bool TefFuseOperations::m_wd_LAN_Active = true;
bool TefFuseOperations::m_wd_WIFI_Active = true;
bool TefFuseOperations::flagHaltEnable= false;

using namespace std;

TefFuseOperations::TefFuseOperations()
{

}

int TefFuseOperations::power_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0)
    {
        stbuf->st_mode = S_IFDIR | 0755;  // mode
        stbuf->st_nlink = 2;              // number of hard links .. in this case (./) and (../)
    }
    else if (strcmp(path, PATH_WD_START) == 0)
    {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = LEN_INFO;
    }
    else if (strcmp(path, PATH_WD_STOP) == 0)
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = LEN_INFO;
    }
    else if (strcmp(path, PATH_WD_ALIVE) == 0)
    {
        stbuf->st_mode = S_IFREG | 0666;
        stbuf->st_nlink = 1;
        stbuf->st_size = LEN_INFO;
    }
    else if (strcmp(path, PATH_WD_REMTIME) == 0)
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = LEN_INFO;
    }
    else if (strcmp(path, PATH_WD_VERSION) == 0)
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = LEN_INFO;
    }
    else
        return res = -ENOENT;

    return res;
}

int TefFuseOperations::power_readDir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    if(strcmp(path, "/") == 0)
    {
        filler(buf,  ".",                        NULL, 0);
        filler(buf,  "..",                       NULL, 0);
        filler(buf,  PATH_WD_START          + 1, NULL, 0);
        filler(buf,  PATH_WD_STOP           + 1, NULL, 0);
        filler(buf,  PATH_WD_ALIVE          + 1, NULL, 0);
        filler(buf,  PATH_WD_REMTIME        + 1, NULL, 0);
        filler(buf,  PATH_WD_VERSION        + 1, NULL, 0);
    }
    else
        return -ENOENT;

    return 0;
}

int TefFuseOperations::power_read(const char *path, char *buf, size_t size, off_t offset, fuse_file_info *fi)
{
    (void) fi;

    bool powered;

    if(offset > 1)
        return 0;

    if(size < 1)
        return 0;
    if (strcmp(path, PATH_WD_START) == 0)
    {
        string app= GetWdON_Info();
        int chwr =snprintf(buf, size, app.c_str());
        return chwr;
    }
    else if (strcmp(path, PATH_WD_STOP) == 0)
    {
        string app= GetWdON_Info();
        int chwr =snprintf(buf, size, app.c_str());
        return chwr;
    }
    else if (strcmp(path, PATH_WD_ALIVE) == 0)
    {
        powered = GetAlive();
    }
    else if (strcmp(path, PATH_WD_REMTIME) == 0)
    {
        string app= GetRemainTime();
        int chwr =snprintf(buf, size, app.c_str());
        return chwr;
    }
    else if (strcmp(path, PATH_WD_VERSION) == 0)
    {
        string app= GetDriverVersion();
        int chwr =snprintf(buf, size, app.c_str());
        return chwr;
    }
    else
        return -ENOENT;

    if(powered)
        buf[0] = '1';
    else
        buf[0] = '0';

    return 1;
}

int TefFuseOperations::power_write(const char *path, const char *buf, size_t size, off_t offset, fuse_file_info *fi)
{
    (void) fi;

    if(offset > 1)
    {
        return 0;
    }

    if(size < 1)
    {
        return 0;
    }

    if (strcmp(path, PATH_WD_START) == 0)
    {
        string str(buf);
        set_initTime(str);
    }
    else if (strcmp(path, PATH_WD_STOP) == 0)
    {
        SetWdON(false);
    }
    else if (strcmp(path, PATH_WD_ALIVE) == 0)
    {
        SetAlive(true);
    }
    else
    {
        return -ENOENT;
    }

    return size;
}

void TefFuseOperations::SetAlive(bool alive_on)
{
    pthread_mutex_lock(&m_mutex_wd_Alive);
    m_wd_Alive = alive_on;
    pthread_mutex_unlock(&m_mutex_wd_Alive);
}

void TefFuseOperations::SetWdON(bool alive_on)
{
    pthread_mutex_lock(&m_mutex_wd_status);
    m_wd_Active = alive_on;
    pthread_mutex_unlock(&m_mutex_wd_status);
}


bool TefFuseOperations::GetAlive(void)
{
    bool outst =false;
    pthread_mutex_lock(&m_mutex_wd_Alive);
    outst = m_wd_Alive;
    pthread_mutex_unlock(&m_mutex_wd_Alive);
    return(outst);
}

bool TefFuseOperations::GetWdON(void)
{

    bool outst =false;
    pthread_mutex_lock(&m_mutex_wd_status);
    outst =m_wd_Active;
    pthread_mutex_unlock(&m_mutex_wd_status);

    return(outst);
}

string TefFuseOperations::GetDriverVersion(void)
{
    return VERSION_DRIVER;
}

string TefFuseOperations::GetRemainTime(void)
{
    bool outst =false;
    string info;
    uint32_t eth0val=0;
    uint32_t wlanval=0;
    uint32_t val=0;

    outst =GetWdON();

    info = "\n";

    if(outst)
    {
        eth0val=uiwTimeEth;
        wlanval=uiwTimeWifi;

       if(m_wd_LAN_Active && m_wd_WIFI_Active )
       {
            if(eth0val > 0 && eth0val < wlanval )
            {
                val=eth0val;
            }
            else
            {
               if(wlanval >  0)
               {
                 val=wlanval;
               }
            }
        }
        else if (m_wd_LAN_Active)
        {
            val=eth0val;
        }
        else if (m_wd_WIFI_Active)
        {
            val=wlanval;
        }
        else
        {
            info = "\n";
            return(info);
        }
        if(val > 0)
        {
            val = val - 1;
        }
        val = val * 60;
        val = val + uiwSecTime;
        if(val > 0)
        {
            info = std::to_string( val )+"\n";
        }
    }
    printf("%s",info.c_str());

    return(info);
}


string TefFuseOperations::GetWdON_Info(void)
{

    bool outst =false;
    string info;
    outst =GetWdON();
    if(outst)
    {
        //Enable :nttEth-Res = 1440ntt Wifi-Res = 1440nttEth-Time = 1440nttWifi-Time = 1440n
        info = "Enable :\n\t";
        info = info +"Eth-Res = "  + std::to_string( uiwTimeEth_ResetValue  ) +"\n\t";
        info = info +"Wifi-Res = " + std::to_string( uiwTimeWifi_ResetValue ) +"\n\t";
        info = info +"Eth-Time = " + std::to_string( uiwTimeEth  )+"\n\t";
        info = info +"Wifi-Time = " +std::to_string( uiwTimeWifi )+"\n\t";
        info = info +"SecDCount = " +std::to_string( uiwSecTime )+"\n\t";
        info = info +"^^^^^^^^^\n";
    }
    else
    {
        info = " is Disable\n";
    }

    printf("%s",info.c_str());

    return(info);
}

void* TefFuseOperations::wdog_thread(void* x)
{
    (void)x;
    bool bbAlive;
    bool power_on;

    while(1)
    {
        sleep(1); // <= timer attesa di un secondo
        // Controllo richietsa accensione
        power_on = GetWdON();

        if(power_on)
        {
            m_wd_Loop=true;
            bbAlive= GetAlive();
            if(bbAlive)
            {
                SetAlive(false);
                reset_countdown();
            }
            flow_coutdown();
        }
        else
        {
            m_wd_Loop=false;
        }
    }
}

void *TefFuseOperations::power_init_fs (struct fuse_conn_info *conn)
{
    (void)conn;

    pthread_create(&m_thread,NULL,wdog_thread,NULL );

    return NULL;
}
bool TefFuseOperations::startPthreads()
{
    if(pthread_mutex_init(&m_mutex_wd_Alive ,  NULL) != 0 ||
            pthread_mutex_init(&m_mutex_wd_status , NULL) != 0)
    {
        return false;
    }
    return true;
}

bool TefFuseOperations::reset_countdown()
{
    uiwTimeEth  = uiwTimeEth_ResetValue;
    uiwTimeWifi   = uiwTimeWifi_ResetValue;
    uiwSecTime  = SECOND_IN_MIN;
    printf ("Countdown resetted !!! ");
    return true;
}

bool TefFuseOperations::flow_coutdown()
{
    int ethres=20;
    int wifres=20;

    if(uiwSecTime > 0)
    {
        uiwSecTime--;
    }
    else
    {

        ethres = action_Ethetnet();
        wifres = action_Wifi();
        CloseNode();
        if(ethres == 0)
        {
            uiwSecTime  = SECOND_IN_MIN;
        }
        if(wifres == 0)
        {
            uiwSecTime  = SECOND_IN_MIN;
        }
        if(uiwSecTime < 1)
        {
            SetWdON(false);
        }

    }
    return true;
}



bool TefFuseOperations::set_initTime  ( string parameter )
{
    vector<string> v =split( parameter, ' ' );

    string TimeEth=v[0];
    string TimeWiFi=v[1];
    string HaltEnable=v[2];

    string::size_type sz;   // alias of size_t

    uint32_t teth = stoi (TimeEth,&sz);
    uint32_t tewf = stoi (TimeWiFi,&sz);
    uint32_t tefhalt = stoi (HaltEnable,&sz);

    SetWdON(false);
    for (int i=0;i<10 && m_wd_Loop==true;i++)
    {
        sleep(1);
    }

    if (teth < TIME_MIN_ACTIVATION)
    {
        teth=0;
    }
    else if (teth > TIME_MAX_ACTIVATION)
    {
        teth = TIME_MAX_ACTIVATION;
    }


    if (tewf < TIME_MIN_ACTIVATION)
    {
        tewf=0;
    }
    else if (tewf > TIME_MAX_ACTIVATION)
    {
        tewf = TIME_MAX_ACTIVATION;
    }

    flagHaltEnable = false;
    if (tefhalt == 1)
    {
        flagHaltEnable = true;
    }
    uiwTimeEth_ResetValue  = teth;
    uiwTimeWifi_ResetValue = tewf;

    reset_countdown();
    if(uiwTimeEth_ResetValue > 0 || uiwTimeWifi_ResetValue > 0)
    {
        SetWdON(true);
    }

    return(true);
}

vector<string> TefFuseOperations::split(string str, char delimiter)
{
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}


int TefFuseOperations::action_Ethetnet  ( void )
{
    if (!uiwTimeEth_ResetValue)
    {
        printf( "Ethernet UP ");
        return 100;
    }
    else
    {
        if(uiwTimeEth > 0)
        {
            uiwTimeEth--;
        }
        if(uiwTimeEth < 1)
        {
            printf( "Ethernet DOWN ");
            if(DisAbleNetwork(LAN)==1)
            {
                m_wd_LAN_Active=false;
                 return 1;
            };
        }

    }
    return 0;
}

int TefFuseOperations::action_Wifi( void )
{
    if (!uiwTimeWifi_ResetValue)
    {
        printf( "Wifi UP ");
        return 100;
        // no Action WIFI UP by Hilse
    }
    else
    {
        if(uiwTimeWifi > 0)
        {
            uiwTimeWifi--;
        }

        if(uiwTimeWifi < 1)
        {
            printf( "Wifi DOWN ");
            if(DisAbleNetwork(WIFI)==1)
            {
                m_wd_WIFI_Active=false;
                return 1;
            };
        }

    }
    return 0;
}

int TefFuseOperations::DisAbleNetwork(NET_TYPE net_off)
{
    std::string name;
    //static char auxString[256];

    switch(net_off)
    {
    case LAN:
        name = (std::string)LAN_OFF;
        printf( " LAN Connection DOWN !");
        system("/bin/bash /usr/bin/lan_off.sh &");
        break;
    case WIFI:
        name = (std::string)WIFI_OFF;
        printf( " WIFI Connection DOWN !");
        system("/bin/bash /usr/bin/wifi_off.sh &");
        break;
    default:
        return -1;
    }

    //    sprintf(auxString, "/bin/bash %s &", name.c_str());
    //    system(auxString);
    sleep(1);
    return 1;
}

int TefFuseOperations::CloseNode(void)
{

    if(!m_wd_LAN_Active && !m_wd_WIFI_Active)
    {
        for(int i=0; i<2; i++)
        {
            system("killall node");
            pthread_mutex_lock(&m_mutex_wd_status);
            m_wd_Active=false;
            pthread_mutex_unlock(&m_mutex_wd_status);

            sleep(1);
        }
        if(flagHaltEnable)
        {
            system("echo 0 > /dev/dmg/out_arm_active");
            system("halt");
        }

        return 1;
    }
    return 0;
}
