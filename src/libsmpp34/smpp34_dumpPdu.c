/*
 * Copyright (C) 2006 Movilgate SRL.
 * File  : smpp34_dumpPdu.c
 * Author: Raul Tremsal <rtremsal@movilgate.com>
 *
 * This file is part of libsmpp34 (c-open-smpp3.4 library).
 *
 * The libsmpp34 library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>

#ifdef __linux__
#include <netinet/in.h>
#endif

#include "smpp34.h"
#include "smpp34_structs.h"
#include "smpp34_params.h"

#if _MSC_VER > 1000
#pragma warning(disable: 4996) // '_snprintf': This function or variable may be unsafe. Consider using _snprintf_s instead.
#endif

/* FUNCTIONS ******************************************************************/
int
smpp34_dumpPdu(uint32_t type, uint8_t *dest, int size_dest, void* tt)
{

    char dummy_b[SMALL_BUFF];
    int left = size_dest;
    int lenval = 0;
    char l_dest[512];
    int lefterror = 0;
	 
	smpp34_err_init();
	lefterror = smpp34_error_size;

    memset(l_dest, 0, sizeof(l_dest));

#define instancia t1->

#define U32( inst, par, _str ){\
    _str(inst par, dummy_b);\
    if( strcmp( dummy_b, "" ) == 0 ){\
        PUTLOG("[%s:%08X(%s)]", par, inst par, "Value is invalid");\
    } else {\
        PUTLOG("[%s:%08X(%s)]", par, inst par, "OK");\
    };\
    lenval = snprintf( (char *)l_dest, sizeof(l_dest), "%-30s[%08X] - [%s]\n",\
                                                    #par,inst par,dummy_b);\
    if( lenval >= left ){\
        PUTLOG("[%s:%08X(%s)]", par, inst par,\
                                      "Value lenght exceed buffer lenght");\
        return( -1 );\
    };\
    snprintf( (char *)dest, left, "%s", l_dest);\
    left -= lenval; dest += lenval;\
}

#define U16( inst, par, _str ){\
    _str(inst par, dummy_b);\
    if( strcmp( dummy_b, "" ) == 0 ){\
        PUTLOG("[%s:%04X(%s)]", par, inst par, "Value is invalid");\
    } else {\
        PUTLOG("[%s:%04X(%s)]", par, inst par, "OK");\
    };\
    lenval = snprintf( (char *)l_dest, sizeof(l_dest),\
                               "%-30s[%04X] [%s]\n",#par,inst par,dummy_b);\
    if( lenval >= left ){\
        PUTLOG("[%s:%04X(%s)]", par, inst par,\
                                      "Value lenght exceed buffer lenght");\
        return( -1 );\
    };\
    snprintf( (char *)dest, left, "%s", l_dest);\
    left -= lenval; dest += lenval;\
}

#define U08( inst, par, _str ){\
    _str(inst par, dummy_b);\
    if( strcmp( dummy_b, "" ) == 0 ){\
        PUTLOG("[%s:%02X(%s)]", par, inst par, "Value is invalid");\
    } else {\
        PUTLOG("[%s:%02X(%s)]", par, inst par, "OK");\
    };\
    lenval = snprintf( (char *)l_dest, sizeof(l_dest), "%-30s[%02X]       - [%s]\n",\
                                                      #par,inst par, dummy_b);\
    if( lenval >= left ){\
        PUTLOG("[%s:%02X(%s)]", par, inst par,\
                                      "Value lenght exceed buffer lenght");\
        return( -1 );\
    };\
    snprintf( (char *)dest, left, "%s", l_dest);\
    left -= lenval; dest += lenval;\
}

#define _op(inst, par, size ){\
    lenval = snprintf( (char *)l_dest, sizeof(l_dest), "%-30s[%."#size"s]\n",\
                                                           #par, inst par);\
    if( lenval >= left ){\
        PUTLOG("[%s:%s(%s)]", par, inst par, \
                                     "Value lenght exceed buffer lenght");\
        return( -1 );\
    };\
    PUTLOG("[%s:%s(%s)]", par, inst par, "OK");\
    snprintf( (char *)dest, left, "%s", l_dest);\
    left -= lenval; dest += lenval;\
}

#define C_OCTET( inst, par, size ) {\
    _op(inst, par, size )\
}

#define OCTET8( inst, par, size ){\
    int i = 0;\
    char *p =  (char *)l_dest;\
    int dummy = 0;\
    lenval = *((inst par) - 1);\
    if( (lenval + 33) >= left ){\
        PUTLOG("[%s:%s(%s)]", par, inst par, \
                                     "Value lenght exceed buffer lenght");\
        return( -1 );\
    };\
    PUTLOG("[%s:%s(%s)]", par, "<bin>", "OK");\
    p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), "%-30s[", #par);\
    for(i = 0 ; i < lenval; i++){\
        if( *((inst par)+i) < ' ' || *((inst par)+i) > '~' ){\
            p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), ".");\
        } else {\
            p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), "%c",\
                                                 *((inst par)+i));\
        };\
    };\
    p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), "]\n");\
    dummy = snprintf( (char *)dest, left, "%s", l_dest);\
    left -= dummy; dest += dummy;\
};


#define OCTET16( inst, par, size ){\
    int i = 0;\
    char *p =  (char *)l_dest;\
    int dummy = 0;\
    uint16_t l_lenval = 0;\
    memcpy(&l_lenval, ((inst par) - sizeof(uint16_t)), sizeof(uint16_t));\
    if( (l_lenval + 33) >= left ){\
        PUTLOG("[%s:%s(%s)]", par, inst par, \
                                     "Value lenght exceed buffer lenght");\
        return( -1 );\
    };\
    PUTLOG("[%s:%s(%s)]", par, "<bin>", "OK");\
    p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), "%-30s[", #par);\
    for( i = 0; i < l_lenval; i++){\
        if( *((inst par)+i) < ' ' || *((inst par)+i) > '~' ){\
            p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), ".");\
        } else {\
            p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), "%c",\
                                                 *((inst par)+i));\
        };\
    };\
    p += snprintf(p, (sizeof(l_dest)-(p-l_dest)), "]\n");\
    dummy = snprintf( (char *)dest, left, "%s", l_dest);\
    left -= dummy; dest += dummy;\
};


#define TLV( inst, tlv1, do_tlv ){\
    tlv_t *aux_tlv = inst tlv1;\
    while( aux_tlv != NULL ){\
        do_tlv( aux_tlv );\
        aux_tlv = aux_tlv->next;\
    };\
};

#define DAD( inst, dad1, do_dad ){\
    dad_t *aux_dad = inst dad1;\
    while( aux_dad != NULL ){\
        do_dad( aux_dad );\
        aux_dad = aux_dad->next;\
    };\
};

#define UDAD( inst, udad1, do_udad ){\
    udad_t *aux_udad = inst udad1;\
    while( aux_udad != NULL ){\
        do_udad( aux_udad );\
        aux_udad = aux_udad->next;\
    };\
};

#include "def_frame/alert_notification.tlv"
#include "def_frame/bind_receiver_resp.tlv"
#include "def_frame/bind_transceiver_resp.tlv"
#include "def_frame/bind_transmitter_resp.tlv"
#include "def_frame/data_sm.tlv"
#include "def_frame/data_sm_resp.tlv"
#include "def_frame/deliver_sm.tlv"
#include "def_frame/submit_multi_resp.udad"
#include "def_frame/submit_multi.dad"
#include "def_frame/submit_multi.tlv"
#include "def_frame/submit_sm.tlv"
#include "def_list/smpp34_protocol.def"

#include "def_frame/clean.frame"

    return( 0 );
};


