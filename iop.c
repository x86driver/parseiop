#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mt3329_types.h"
#include "UTL_func.h"

void parse_remote_packet(unsigned char *buf)
{
    GPS_packed_remote_state_type remote_packet;
    memcpy(&remote_packet, buf+3, sizeof(GPS_packed_remote_state_type));
    time_t utcsec = (time_t) UTL_get_time(remote_packet.tow,
                                          remote_packet.wn_lng,
                                          remote_packet.leap_scnds);

    printf("time: %s, fix: %d\n", ctime(&utcsec), remote_packet.fix);
}

void parse_iop(unsigned char *buf)
{
    switch (buf[1]) {
    case 0x99:
//        printf("IOP 153: IOP_PVT_REMOTE_STATE\n");
        parse_remote_packet(buf);
        break;
    case 0x9a:
//            printf("IOP 154: IOP_PVT_REMOTE_SAT\n");
        break;
    default:
//            printf("IOP %d\n", buf[1]);
        break;
    }
}

int main()
{
    unsigned char buf[256];
    unsigned char *ptr = &buf[0];
    FILE *fp = fopen("gps.log", "r");
    if (!fp) {
        perror("fopen");
        exit(1);
    }

    while (!feof(fp)) {
        memset(&buf[0], 0, sizeof(buf));
        ptr = &buf[0];
        fread(ptr, 3, 1, fp);
        ptr += 3;

        while (!feof(fp)) {
            fread(ptr++, 1, 1, fp);
            if (*(ptr-2) == 0x10 && (*(ptr-1) == 0x03))
                break;
        }
        parse_iop(buf);
    }

    fclose(fp);
    return 0;
}
