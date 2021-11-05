#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "usbdevice.h"

#define USB_DEVICES_INFO_PATH   "/sys/bus/usb/devices"

/**
 * @description: check whether the last char is <c> in string <s>
 * @param s   string for judge
 * @param c   charater that you want to check
 * @return: NULL:no find or s is NULL   address of s:have find 
 */
static inline char *last_char_is(const char *s, int c)
{
	if (s && *s) 
    {
		int sz = strlen(s) - 1;
		s += sz;
		if ( (unsigned char)*s == c)
			return (char*)s;
	}

	return NULL;
}

/**
 * @description: append path information
 * @param path		first path information 
 * @param temppath	second path information
 * @param fileneme	thrid path information
 * @return: success: a int8_t address of path information which have append path info
 *      	error: NULL
 */
static inline char *concat_path_file(const char *first_path, const char *second_path, const char *third_path)
{
	char *lc, *lt;
    static char uevent_filename[50];

	if (!first_path)
		first_path = "";
	lc = last_char_is(first_path, '/');
    if (!second_path)
		first_path = "";
	lt = last_char_is(second_path, '/');
    
	while (*third_path == '/')
		third_path++;

    while (*second_path == '/')
		second_path++;    

    memset(uevent_filename, 0, sizeof(uevent_filename));
    sprintf(uevent_filename, "%s%s%s%s%s", first_path, (lc == NULL ? "/" : ""), second_path, (lt == NULL ? "/" : ""), third_path);

	return uevent_filename;
}

static inline int get_device_id(const char *srcdata, char *tmp_vid, char *tmp_did)
{
    char vids = 0;
    char dids = 0;
    unsigned char counter = 0;

    while (srcdata && *srcdata) {
        if (*srcdata == '/') {
            vids = 0;
            dids = 1;
            counter = 0;
        }

        if ((*srcdata == '/') && tmp_did[0]) {
            dids = 0;
            break;
        }
        
        if (vids)
            tmp_vid[counter++] = *srcdata;

        if (dids && (*srcdata != '/'))
            tmp_did[counter++] = *srcdata;

        if (*srcdata == '=')
            vids = 1;

        srcdata++;
    }

    return (int)*srcdata;
}

static inline bool compare_devID_rowData(FILE *fp, const char *vid, const char *did)
{
    char rowinfo[128] = {0};
    char vidinfo[5] = {0};
    char didinfo[5] = {0};

    while (fgets(rowinfo, sizeof(rowinfo), fp) != NULL) {
        if (strstr(rowinfo, "PRODUCT") != NULL) {
            get_device_id(rowinfo, vidinfo, didinfo);

            if ((strstr(vidinfo, vid)) && (strstr(didinfo, did)))
                return true;
        }
    }

    return false;
}


/**
 * @description: check lte status and types
 * @param none
 * @return: enum MAC_STATUS
 */
bool check_usbdevice_status(const char *vid, const char *did)
{
    bool ltemac_state = false;
    FILE *fp = NULL;
    DIR *dirp = NULL;
    struct dirent *dinfo = NULL;
    struct stat statbuf;
    char *usb_filename = NULL;
    char temp_pathname[32] = {0};

    if ((dirp = opendir(USB_DEVICES_INFO_PATH)) == NULL)
        return ltemac_state;
  
    while ((dinfo = readdir(dirp)) != NULL) {
        if (dinfo->d_name[0] == '.') 
            continue;

        memset(temp_pathname, 0, sizeof(temp_pathname));
        sprintf((char *)temp_pathname, "%s/%s", USB_DEVICES_INFO_PATH, dinfo->d_name);
        if (stat(temp_pathname, &statbuf) != 0) 
            continue;

        if (S_ISDIR(statbuf.st_mode)) {
            usb_filename = concat_path_file(USB_DEVICES_INFO_PATH, dinfo->d_name, "/uevent");
            if ((fp = fopen(usb_filename, "r"))) {
                if (compare_devID_rowData(fp, vid, did)) {
                    ltemac_state = true;
                    break;
                }
            }
            fclose(fp);
        }
    }
    
    closedir(dirp);

    return ltemac_state;
}

bool check_usbport_status(const char *port)
{
    if (!port)
        return false;
    
    int r = access(port, R_OK|W_OK);
    
    return (r >= 0) ? true : false;
}


