/* $Id: macproxy.c,v 1.2 2006/06/01 12:31:52 amura Exp $
 *
 * Macintosh Proxy Dispatch routines
 *
 * Copyright (C) 2006 MURAMATSU Atsushi <amura@tomato.sakura.ne.jp>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <stddef.h>
#include "macproxy.h"
#include <SystemConfiguration/SystemConfiguration.h> 

/**
 * Get setting by C string.
 * This code is based on a sample code from Apple inc.
 *
 * @param type Proxytype (HTTP, HTTPS, FTP is supported)
 * @param host result: proxy host name
 * @param hostSize size of "host"
 * @param port result: port number of proxy
 * @return proxy is enable or not
 */
int
GetProxySetting(int type, char *host, size_t hostSize, int *port)
{
    int result;
    CFDictionaryRef proxyDict;
    CFNumberRef enableNum;
    int enable;
    CFStringRef hostStr;
    CFNumberRef portNum;
    int portInt;
    
    CFStringRef proxyEnableKey, proxyNameKey, proxyPortKey;

    assert(host != NULL);
    assert(port != NULL);
    
    switch (type) {
    case SERVICE_HTTP:
	proxyEnableKey = kSCPropNetProxiesHTTPEnable;
	proxyNameKey = kSCPropNetProxiesHTTPProxy;
	proxyPortKey = kSCPropNetProxiesHTTPPort;
	break;

    case SERVICE_HTTPS:
	proxyEnableKey = kSCPropNetProxiesHTTPSEnable;
	proxyNameKey = kSCPropNetProxiesHTTPSProxy;
	proxyPortKey = kSCPropNetProxiesHTTPSPort;
	break;

    case SERVICE_FTP:
	proxyEnableKey = kSCPropNetProxiesFTPEnable;
	proxyNameKey = kSCPropNetProxiesFTPProxy;
	proxyPortKey = kSCPropNetProxiesFTPPort;
	break;

    default:
	assert(1);
    }
    
    /* Get Dictionaly */
    proxyDict = SCDynamicStoreCopyProxies(NULL);
    result = (proxyDict != NULL);
    
    /* Get proxy enable flag. This is not CFBoolean, but CFNumber. */
    if (result) {
        enableNum =
	    (CFNumberRef) CFDictionaryGetValue(proxyDict, proxyEnableKey);

        result = (enableNum != NULL)
	    && (CFGetTypeID(enableNum) == CFNumberGetTypeID());
    }
    if (result) {
        result = CFNumberGetValue(enableNum, kCFNumberIntType, &enable)
	    && (enable != 0);
    }
    
    /*
     * Get proxy hostname. Name of DNS must have ASCII char only.
     * If not ASCII char is exist,  function CFStringGetCString is fault and
     * return false.
     */
    if (result) {
        hostStr = (CFStringRef) CFDictionaryGetValue(proxyDict, proxyNameKey);
        result = (hostStr != NULL)
            && (CFGetTypeID(hostStr) == CFStringGetTypeID());
    }
    if (result)
        result = CFStringGetCString(hostStr, host, (CFIndex)hostSize,
				    kCFStringEncodingASCII);
    
    /* Get port num of proxy */
    if (result) {
        portNum =
	    (CFNumberRef) CFDictionaryGetValue(proxyDict, proxyPortKey);
        result = (portNum != NULL)
            && (CFGetTypeID(portNum) == CFNumberGetTypeID());
    }
    if (result)
        result = CFNumberGetValue(portNum, kCFNumberIntType, &portInt);
    if (result)
        *port = (int)(portInt & 0xffff);

    /* cleanup */
    if (proxyDict != NULL)
        CFRelease(proxyDict);
    if (!result) {
        *host = '\0';
        *port = 0;
    }
    return result;
}
