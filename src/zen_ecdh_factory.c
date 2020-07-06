// Generated by build/codegen_ecdh_factory.sh
// Wed 27 May 2020 12:53:40 PM CEST
#include <string.h>
#include <jutils.h>
#include <lauxlib.h>
#include <zen_ecdh.h>
#include <zen_error.h>
#include <ecdh_GOLDILOCKS.h>

void ecdh_init(ecdh *ECDH) {
	ECDH->fieldsize = EFS_GOLDILOCKS;
	ECDH->hash = HASH_TYPE_GOLDILOCKS;
	ECDH->ECP__KEY_PAIR_GENERATE = ECP_GOLDILOCKS_KEY_PAIR_GENERATE;
	ECDH->ECP__PUBLIC_KEY_VALIDATE	= ECP_GOLDILOCKS_PUBLIC_KEY_VALIDATE;
	ECDH->ECP__SVDP_DH = ECP_GOLDILOCKS_SVDP_DH;
	ECDH->ECP__ECIES_ENCRYPT = ECP_GOLDILOCKS_ECIES_ENCRYPT;
	ECDH->ECP__ECIES_DECRYPT = ECP_GOLDILOCKS_ECIES_DECRYPT;
	ECDH->ECP__SP_DSA = ECP_GOLDILOCKS_SP_DSA;
	ECDH->ECP__VP_DSA = ECP_GOLDILOCKS_VP_DSA;
	act(NULL,"ECDH curve is GOLDILOCKS");
}
