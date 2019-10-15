#include <stdio.h>
#include <string.h>
#include "arpproxy.h"

unsigned int sort_field = SRC;
unsigned int sort_direction;

int str2hex(const char *str, unsigned char *hex)
{
    int i;
    int pos = 0;
    int temp;
    
    for (i = 1; i <= strlen(str); i++) {
	if (str[i] == '\0' || str[i] == ':') {
	    if (i < 2 || str[i - 2] == ':') {
		sscanf(&str[i - 1], "%1X", &temp);
	    } else {
		sscanf(&str[i - 2], "%2X", &temp);
	    }
	    hex[pos] = (unsigned char) temp;
	    pos++;
	}
    }

    return pos;
}


char *hex2str(const unsigned char *hex, char *str) 
{
    int i;
    int pos = 0;
    
    for (i = 0; i < ETHERADDRLEN; i++) { // 마지막 \0은 무시
        sprintf(&str[pos], "%02X:", hex[i]);
	pos += 3;
    }
    str[pos - 1] = '\0';

    return str;
}


int compare_hex(const unsigned char *hex1, const unsigned char *hex2) 
{
    int i;

    for (i = 0; i < ETHERADDRLEN; i++) {
	if (hex1[i] != hex2[i]) {
	    return (hex1[i] - hex2[i]);
	}
    }
    
    return 0;
}


int compare(const void *a, const void *b)
{
    struct lease_info *lhs = (struct lease_info *) a;
    struct lease_info *rhs = (struct lease_info *) b;
    long ret = 0;

    // null은 무조건 뒤로 보냄
    if (lhs == NULL) {
	return 1;
    } else if (rhs == NULL) {
	return -1;
    }

    if (lhs->src == 0) {
	return 1; 
    } else if (rhs->src == 0) {
	return -1;
    } 

#define COMP_SRC(l,r) { if ((l)->src - (r)->src) { ret = memcmp(&(l)->src, &(r)->src, sizeof (unsigned long)); } if (ret) { if (sort_field == SRC && sort_direction == DESC) { return (ret * -1); } else { return ret; } } }
#define COMP_SRC_HA(l,r) { if (memcmp((l)->src_ha, (r)->src_ha, ETHERADDRLEN)) { ret = memcmp((l)->src_ha, (r)->src_ha, ETHERADDRLEN); } if (ret) { if (sort_field == SRC_HA && sort_direction == DESC) { return (ret * -1); } else { return ret; } } }
#define COMP_SRC_REG_TIME(l,r) { if ((l)->src_reg_time - (r)->src_reg_time) { ret = (l)->src_reg_time - (r)->src_reg_time; } if (ret) { if (sort_field == SRC_REG_TIME && sort_direction == DESC) { return (ret * -1); } else { return ret; } } }
#define COMP_PING(l,r) { if ((l)->ping_count - (r)->ping_count) { ret = (l)->ping_count - (r)->ping_count; } if (ret) { if (sort_field == PING && sort_direction == DESC) { return (ret * -1); } else { return ret; } } }
#define COMP_DST(l,r) { if ((l)->dst - (r)->dst) { ret = memcmp(&(l)->dst, &(r)->dst, sizeof (unsigned long)); } if (ret) { if (sort_field == DST && sort_direction == DESC) { return (ret * -1); } else { return ret; } } }
#define COMP_DST_TYPE(l,r) { if ((l)->dst_type == ALIAS && (r)->dst_type == ROUTE) { ret = -1; } else if ((l)->dst_type == ROUTE && (r)->dst_type == ALIAS) { ret = 1; } if (ret) { if (sort_field == DST_TYPE && sort_direction == DESC) { return (ret * -1); } else { return ret; } } }
#define COMP_DST_REG_TIME(l,r) { if ((l)->dst_reg_time - (r)->dst_reg_time) { ret = (l)->dst_reg_time - (r)->dst_reg_time; } if (ret) { if (sort_field == DST_REG_TIME && sort_direction == DESC) { return (ret * -1); } else { return ret; } } }

    switch (sort_field) {
    case SRC:
	COMP_SRC(lhs, rhs);
	COMP_SRC_HA(lhs, rhs);
	COMP_SRC_REG_TIME(lhs, rhs);
	COMP_PING(lhs, rhs);
	COMP_DST_TYPE(lhs, rhs);
	COMP_DST(lhs, rhs);
	COMP_DST_REG_TIME(lhs, rhs);
	break;
    case SRC_HA:
	COMP_SRC_HA(lhs, rhs);
	COMP_SRC(lhs, rhs);
	COMP_SRC_REG_TIME(lhs, rhs);
	COMP_PING(lhs, rhs);
	COMP_DST_TYPE(lhs, rhs);
	COMP_DST(lhs, rhs);
	COMP_DST_REG_TIME(lhs, rhs);
	break;
    case PING:
	COMP_PING(lhs, rhs);
	COMP_SRC(lhs, rhs);
	COMP_SRC_HA(lhs, rhs);
	COMP_SRC_REG_TIME(lhs, rhs);
	COMP_DST_TYPE(lhs, rhs);
	COMP_DST(lhs, rhs);
	COMP_DST_REG_TIME(lhs, rhs);
	break;
    case SRC_REG_TIME:
	COMP_SRC_REG_TIME(lhs, rhs);
	COMP_SRC(lhs, rhs);
	COMP_SRC_HA(lhs, rhs);
	COMP_PING(lhs, rhs);
	COMP_DST_TYPE(lhs, rhs);
	COMP_DST(lhs, rhs);
	COMP_DST_REG_TIME(lhs, rhs);
	break;
    case DST:
	COMP_SRC(lhs, rhs);
	COMP_SRC_HA(lhs, rhs);
	COMP_SRC_REG_TIME(lhs, rhs);
	COMP_PING(lhs, rhs);
	COMP_DST(lhs, rhs);
	COMP_DST_TYPE(lhs, rhs);
	COMP_DST_REG_TIME(lhs, rhs);
	break;
    case DST_TYPE:
	COMP_SRC(lhs, rhs);
	COMP_SRC_HA(lhs, rhs);
	COMP_SRC_REG_TIME(lhs, rhs);
	COMP_PING(lhs, rhs);
	COMP_DST_TYPE(lhs, rhs);
	COMP_DST(lhs, rhs);
	COMP_DST_REG_TIME(lhs, rhs);
	break;
    case DST_REG_TIME:
	COMP_SRC(lhs, rhs);
	COMP_SRC_HA(lhs, rhs);
	COMP_SRC_REG_TIME(lhs, rhs);
	COMP_PING(lhs, rhs);
	COMP_DST_REG_TIME(lhs, rhs);
	COMP_DST(lhs, rhs);
	COMP_DST_TYPE(lhs, rhs);
	break;
    }	
    return 0;
}







