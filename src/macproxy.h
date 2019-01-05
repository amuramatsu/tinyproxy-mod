/* $Id: macproxy.h,v 1.2 2006/06/01 12:31:52 amura Exp $
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
#ifndef __MACPROXY_H__
#define __MACPROXY_H__

#define SERVICE_HTTP	0
#define SERVICE_HTTPS	1
#define SERVICE_FTP	2

#define MAC_PROXYNAME_SIZE 512

int GetProxySetting(int type, char *host, size_t hostSize, int *port);

#endif /* __MACPROXY_H__ */
