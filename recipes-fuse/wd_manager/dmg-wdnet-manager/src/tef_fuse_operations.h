#ifndef TefFUSEOPERATIONS_H
#define TefFUSEOPERATIONS_H


#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <fuse.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stddef.h>
#include <string>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


#include "fuse_operations_interface.h"

#define SECOND_IN_MIN 60L
//#define SECOND_IN_MIN 5L

#define TIME_MIN_ACTIVATION  1
#define TIME_MAX_ACTIVATION  1440

#define DEFAULT_TIME  (5 * SECOND_IN_MIN)

#define VERSION_DRIVER "01.00.002\n"



typedef enum
{
    LAN = 0,
    WIFI,
    NTYPE
} NET_TYPE;



using namespace std;

class TefFuseOperations : public FuseOperationsInterface
{
public:
    TefFuseOperations();

    // FuseOperationsInterface interface
public:
    void *power_init_fs(struct fuse_conn_info *conn);
    int power_getattr(const char *path, struct stat *stbuf);
    int power_readDir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info *fi);
    int power_read(const char *path, char *buf, size_t size, off_t offset, fuse_file_info *fi);
    int power_write(const char *path, const char *buf, size_t size, off_t offset, fuse_file_info *fi);


private:

    pthread_t         m_thread;

    static bool              m_wd_Alive;
    static bool              m_wd_Active;
    static pthread_mutex_t   m_mutex_wd_Alive;
    static pthread_mutex_t   m_mutex_wd_status;
    static bool  m_wd_LAN_Active ;
    static bool  m_wd_WIFI_Active ;


    static uint32_t uiwTimeEth ;
    static uint32_t uiwTimeWifi;
    static uint32_t uiwSecTime ;

    static uint32_t uiwTimeEth_ResetValue ;
    static uint32_t uiwTimeWifi_ResetValue;
    static bool flagHaltEnable;
    static bool m_wd_Loop;

    static  int action_Ethetnet( void );
    static  int action_Wifi     ( void );

    static void SetAlive(bool alive_on);
    static void SetWdON (bool alive_on);
    static bool GetAlive(void);
    static bool GetWdON(void);
    static string GetWdON_Info(void);
    static string GetRemainTime(void);
     static string GetDriverVersion(void);
    static bool reset_countdown(void);
    static bool flow_coutdown  (void);
    static bool set_initTime  ( string parameter );
    static vector<string> split(string str, char delimiter);
    static int DisAbleNetwork(NET_TYPE net_off);
    static int CloseNode(void);

    static void *wdog_thread(void *x);
    bool startPthreads();


};

#endif // TEFFUSEOPERATIONS_H
