// name server operation code
#define OPCODE_QUERY 0x0000
#define OPCODE_REGISTER 0x2800
#define OPCODE_RELEASE 0x3000
#define OPCODE_WACK 0x3800
#define OPCODE_REFRESH 0x4000
#define OPCODE_ALTREFRESH 0x4800
#define OPCODE_MULTIHOMED 0x7800
#define OPCODE_MASK 0x7800


// Ethernet for link layer frame
struct ieee8023 {
    u_int8_t destination[6];
    u_int8_t source[6];
    u_int16_t length;
    
    // There may be located the payload here.

    //u_int8_t trailer[???]; // 0x20 for at least 60 byte length
};


// Ethernet II for IP packet
struct ethernet2 {
    u_int8_t destination[6];
    u_int8_t source[6];
    u_int16_t type;

    // There may be located the payload here.
    
    //u_int8_t trailer[???]; // 0x00 for at least 60 byte length
};


struct llc {
    // dsap와 ssap의 LSB는 각각 Individual(0)/Group(1), Command(0)/Response(1)
    u_int8_t dsap; // 0xf0 for NetBIOS
    u_int8_t ssap;
    // control_field는 packet에 쓸 때, u_int16_t형 변수에 memcpy되고
    // 전송할 때 htons()로 byte alignment 조정해야 함. packet에서 읽을
    // 때에는 ntohs()로 변환하고 memcpy로 struct에 써야 함
    struct control_field_struct {
	u_int8_t type:1;
	u_int8_t n_s:7;
	u_int8_t poll_final:1;
	u_int8_t n_r:;
    };
};



#define NBT
struct name_query_request {
    struct header {
	name_trn_id;
	struct flags {
	    opcode; // 0x0
	    rd; // 1
	    b; // 1 for broadcat, else 0
	};
	qdcount; // 1
    };
    struct question_record {
	question_name; // encoded NBT name
	question_type; // NB (0x0020)
	question_class; // IN (0x0001)
    };
};


struct negative_name_query_response {
    struct header {
	name_trn_id; // same as query request
	struct flags {
	    r; // 1 for response packet
	    opcode; // 0x0 for query
	    aa; // 1
	    rd; // copy rd bit from query request
	    ra; // 1 for reply from NBNS
	    b; // 0
	    rcode; // error code
	};
	ancount; // 1
    };
    struct answer_record {
	rr_name; // encoded NBT nam e
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 0 
	rdlength; // 0
    };
};


struct positive_name_query_response {
    struct header {
	name_trn_id; // same as query request
	struct flags {
	    r; // 1 for response packet
	    opcode; // 0x0 for query
	    aa; // 1
	    rd; // copy rd bit from query request
	    ra; // 1 for reply from NBNS
	    b; // 0
	    rcode; // 0x0
	};
	ancount; // 1
    };
    struct answer_record {
	rr_name; // encoded NBT nam e
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; 
	rdlength; // 6 * number of entries
	struct rdata {
	    struct addr_entry {
		nb_flags {
		    g; // 1 for group, 0 for unique
		    ont; // owner type
		};
		nb_address; // owner's IP address
	    } addr_entry[];
	};
    };
};


struct name_registration_request {
    struct header {
	name_trn_id; // set when packet it transmitted
	struct flags {
	    opcode; // 0x5 for registration
	    rd; // 1
	    b; // 1 for broadcast, else 0
	};
	qdcount; // 1
	arcount; // 1 
    };
    struct question_record {
	question_name; // encoded NBT name to be registered
	question_type; // NB (0x0020)
	question_class; // IN (0x0001)
    };
    struct additional_record {
	rr_name; // 0xC00C
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 0 for broadcast, 3 days for unicast
	rdlength; // 6
	struct rdata {
	    struct nb_flags {
		g; // 0 for group, 1 for unique
		ont; // owner type
// owner type
#define GROUP_BIT 0x8000
#define ONT_B 0x0000
#define ONT_P 0x2000
#define ONT_M 0x4000
#define ONT_H 0x6000
#define ONT_MASK 0x6000
	    };
	    nb_address; // requesting node's IP address
	};
    };
};


struct name_registration_response {
    struct header {
	name_trn_id; // must match REQUEST transaction id
	struct flags {
	    r; // 1 for response packet
	    opcode; // 0x5 for registration
	    aa; // 1
	    rd; // 1
	    ra; // 1
	    rcode; // 0x6 for ACT ERR (negative response)
	    b; // 0 for unicast back to requester
	};
	ancount; // 1
    }
    struct answer_record {
	rr_name; // encoded NBT name
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 0 
	rdlength; // 6
	struct rdata {
	    struct nb_flags {
		g; // 1 for group, 0 for unique
		ont; // owner type
	    };
	    nb_address; // owner's IP address
	};
    };
};
	    

struct wait_for_acknowledgement_response {
    struct header {
	name_trn_id; // must match REQUEST transaction id
	struct flags {
	    r; // 1 for response
	    opcode; // 0x7 for WACK
	    aa; // 1
	};
	ancount; // 1
    };
    struct answer_record {
	rr_name; // encoded NBT name from the request
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 60 in SAMBA
	rdlength; // 2
	rdata; // copy of the two-byte header.flags field 
	// of the original request
    };
};


struct name_refresh_request {
    struct header {
	name_trn_id; // set when packet is transmitted
	struct flags {
	    opcode; // 0x8 or 0x9 (refresh)
	    rd; // 0
	    b; // 0
	};
	qdcount; // 1
	arcount; // 1
    };
    struct question_record {
	question_name; // encoded NBT name to be refreshed
	question_type; // NB (0x0020)
	question_class; // IN (0x0001)
    };
    struct additional_record {
	rr_name; // 0xC00C for label string pointer to question_name;
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // client's default TTL value (3 days)
	rdlength; // 6
	struct rdata {
	    struct nb_class {
		g; // 1 for group, 0 for unique
		ont; // owner type
	    };
	    nb_address; // requesting node's IP address
	};
    };
};


struct name_release_request {
    struct header {
	name_trn_id;
	struct flags {
	    opcode; // 0x6 for release
	    b; // 0 for request, 1 for demand 
	};
	qdcount; // 1
	arcount; // 1
    };
    struct question_record {
	question_name; // encoded NBT name
	question_type; // NB (0x0020)
	question_class; // IN (0x0001)
    };
    struct additional_record {
	rr_name; // 0xC00C (label string pointer to question_name
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 0 
	rdlength; // 6
	struct rdata {
	    struct nb_class {
		g; // 1 for group, 0 for unique
		ont; // owner type
	    };
	    nb_address; // releasing node's IP address
	};
    };
};    


struct name_release_response {
    struct header {
	name_trn_id; // must match request transaction id
	struct flags {
	    r; // 1 for response packet
	    opcode; // 0x6 for release
	    aa; // 1
	    rcode; 
	    b; // 0
	};
	ancount; // 1
    }; 
    struct answer_record {
	rr_name; // encoded released name 
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 0 for no meaning
	rdlength; // 6
	rdata; // same as request packet
    };
};


struct node_status_request {
    struct header {
	name_trn_id; // set when packet is transmitted
	struct flags {
	    opcode; // 0x0 for query
	    b; // 0 
	};
	qdcount; // 1
    }
    struct question_record {
	question_name; // encoded NBT name to be queried
	question_type; // NBSTAT (0x0021)
	question_class; // IN (0x0001)
    };
};


struct node_status_response {
    struct header {
	name_trn_id; // same as request ID
	struct flags {
	    r; // true
	    opcode; // 0x0 for query
	    aa; // 1
	};
	ancount = 1;
    };
    struct answer_record {
	rr_name; // the queried name, copied from the request
	rr_type; // NBSTAT (0x0021)
	rr_class; // IN (0x0001)
	ttl; // 0 for no meaning
	rdlength; // total length of following fields
	struct rdata {
	    num_names; // number of node_name[] entries
	    struct node_name {
		netbios_name // 16 octet NetBIOS name, unencoded
		name_flags;
	    } node_name[];
	    statistics; 
	};
    };
};


struct name_conflict_demand {
    struct header {
	name_trn_id; // whatever you like
	struct flags {
	    r; // 1
	    opcode; // 0x5 registration
	    aa; // 1
	    rd; // 1
	    ra; // 1
	    rcode; // CFT_ERR 0x7
	    b; // 0
	};
    };
    struct answer_record {
	rr_name; // NBT name owned by the target node
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 0
	rdlength; // 6
	struct rdata {
	    struct nb_flags {
		g; // 1 for group, 0 for unique
		ont; // owner type
	    };
	    nb_address; // owner's IP address
	};
    };
};


struct name_release_request_demand {
    struct header {
	name_trn_id; // set when packet is transmitted
	struct flags {
	    opcode; // 0x6 for release
	    b; // 0
	};
	qdcount; // 1
	arcount; // 1
    };
    struct question_record {
	question_name; // encoded NBT name to be released
	question_type; // NB (0x0020)
	question_class; // IN (0x0001)
    };
    struct additional_record {
	rr_name; // 0xC00C for label string pointer to question_name
	rr_type; // NB (0x0020)
	rr_class; // IN (0x0001)
	ttl; // 0
	rdlength; // 6
	struct rdata {
	    struct nb_flags {
		g; // 1 for group, 0 for unique
		ont; // owner type
	    };
	    nb_address; // target node's IP address
	};
    };
};    

#endif // NBT
