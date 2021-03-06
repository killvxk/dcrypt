﻿#include <windows.h>
#ifdef SMALL_CODE
	#include "pkcs5_small.h"
	#include "sha512_small.h"
#else
	#include "pkcs5.h"
	#include "sha512.h"
#endif

static const struct {
	const char *key;
	const char *data;
	const char *hmac;
} sha512_hmac_vectors[] = { /* see http://www.rfc-editor.org/rfc/rfc4231.txt */
	{
		"\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
		"Hi There",
		"\x87\xaa\x7c\xde\xa5\xef\x61\x9d\x4f\xf0\xb4\x24\x1a\x1d\x6c\xb0\x23\x79\xf4\xe2"
		"\xce\x4e\xc2\x78\x7a\xd0\xb3\x05\x45\xe1\x7c\xde\xda\xa8\x33\xb7\xd6\xb8\xa7\x02"
		"\x03\x8b\x27\x4e\xae\xa3\xf4\xe4\xbe\x9d\x91\x4e\xeb\x61\xf1\x70\x2e\x69\x6c\x20"
		"\x3a\x12\x68\x54"
	},
	{
		"Jefe",
		"what do ya want for nothing?",
		"\x16\x4b\x7a\x7b\xfc\xf8\x19\xe2\xe3\x95\xfb\xe7\x3b\x56\xe0\xa3\x87\xbd\x64\x22"
		"\x2e\x83\x1f\xd6\x10\x27\x0c\xd7\xea\x25\x05\x54\x97\x58\xbf\x75\xc0\x5a\x99\x4a"
		"\x6d\x03\x4f\x65\xf8\xf0\xe6\xfd\xca\xea\xb1\xa3\x4d\x4a\x6b\x4b\x63\x6e\x07\x0a"
		"\x38\xbc\xe7\x37"
	},
	{
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
		"\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
		"\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
		"\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd",
		"\xfa\x73\xb0\x08\x9d\x56\xa2\x84\xef\xb0\xf0\x75\x6c\x89\x0b\xe9\xb1\xb5\xdb\xdd"
		"\x8e\xe8\x1a\x36\x55\xf8\x3e\x33\xb2\x27\x9d\x39\xbf\x3e\x84\x82\x79\xa7\x22\xc8"
		"\x06\xb4\x85\xa4\x7e\x67\xc8\x07\xb9\x46\xa3\x37\xbe\xe8\x94\x26\x74\x27\x88\x59"
		"\xe1\x32\x92\xfb"
	},
	{
		"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14"
		"\x15\x16\x17\x18\x19",
		"\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
		"\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
		"\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd",
		"\xb0\xba\x46\x56\x37\x45\x8c\x69\x90\xe5\xa8\xc5\xf6\x1d\x4a\xf7\xe5\x76\xd9\x7f"
		"\xf9\x4b\x87\x2d\xe7\x6f\x80\x50\x36\x1e\xe3\xdb\xa9\x1c\xa5\xc1\x1a\xa2\x5e\xb4"
		"\xd6\x79\x27\x5c\xc5\x78\x80\x63\xa5\xf1\x97\x41\x12\x0c\x4f\x2d\xe2\xad\xeb\xeb"
		"\x10\xa2\x98\xdd"
	},
	{
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
		"Test Using Larger Than Block-Size Key - Hash Key First",
		"\x80\xb2\x42\x63\xc7\xc1\xa3\xeb\xb7\x14\x93\xc1\xdd\x7b\xe8\xb4\x9b\x46\xd1\xf4"
		"\x1b\x4a\xee\xc1\x12\x1b\x01\x37\x83\xf8\xf3\x52\x6b\x56\xd0\x37\xe0\x5f\x25\x98"
		"\xbd\x0f\xd2\x21\x5d\x6a\x1e\x52\x95\xe6\x4f\x73\xf6\x3f\x0a\xec\x8b\x91\x5a\x98"
		"\x5d\x78\x65\x98"
	},
	{
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
		"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
		"This is a test using a larger than block-size key and a larger than block-size d"
		"ata. The key needs to be hashed before being used by the HMAC algorithm.",
		"\xe3\x7b\x6a\x77\x5d\xc8\x7d\xba\xa4\xdf\xa9\xf9\x6e\x5e\x3f\xfd\xde\xbd\x71\xf8"
		"\x86\x72\x89\x86\x5d\xf5\xa3\x2d\x20\xcd\xc9\x44\xb6\x02\x2c\xac\x3c\x49\x82\xb1"
		"\x0d\x5e\xeb\x55\xc3\xe4\xde\x15\x13\x46\x76\xfb\x6d\xe0\x44\x60\x65\xc9\x74\x40"
		"\xfa\x8c\x6a\x58"
	}
};

static const struct {
  int          i_count;
  const char  *password;
  const char  *salt;
  int          dklen;
  const char  *key;
} pkcs5_vectors[] = {
	{ 5, "password", "\x12\x34\x56\x78", 4, "\x13\x64\xae\xf8" },
	{ 5, "password", "\x12\x34\x56\x78", 144, "\x13\x64\xae\xf8\x0d\xf5\x57\x6c\x30\xd5\x71\x4c\xa7\x75\x3f"
	"\xfd\x00\xe5\x25\x8b\x39\xc7\x44\x7f\xce\x23\x3d\x08\x75\xe0\x2f\x48\xd6\x30\xd7\x00\xb6\x24\xdb\xe0\x5a\xd7\x47\xef\x52"
	"\xca\xa6\x34\x83\x47\xe5\xcb\xe9\x87\xf1\x20\x59\x6a\xe6\xa9\xcf\x51\x78\xc6\xb6\x23\xa6\x74\x0d\xe8\x91\xbe\x1a\xd0\x28"
	"\xcc\xce\x16\x98\x9a\xbe\xfb\xdc\x78\xc9\xe1\x7d\x72\x67\xce\xe1\x61\x56\x5f\x96\x68\xe6\xe1\xdd\xf4\xbf\x1b\x80\xe0\x19"
	"\x1c\xf4\xc4\xd3\xdd\xd5\xd5\x57\x2d\x83\xc7\xa3\x37\x87\xf4\x4e\xe0\xf6\xd8\x6d\x65\xdc\xa0\x52\xa3\x13\xbe\x81\xfc\x30"
	"\xbe\x7d\x69\x58\x34\xb6\xdd\x41\xc6" }
};

int test_pkcs5()
{
	const char *p_key, *data;
	char        hmac[SHA512_DIGEST_SIZE];
	const char *pass, *salt;
	int         i, dklen;	
	char        dk[144];

	// test HMAC-SHA-512 
	for (i = 0; i < _countof(sha512_hmac_vectors); i++) 
	{
		p_key = sha512_hmac_vectors[i].key;
		data  = sha512_hmac_vectors[i].data;

		sha512_hmac(p_key, strlen(p_key), data, strlen(data), hmac);
		if (memcmp(hmac, sha512_hmac_vectors[i].hmac, sizeof(hmac)) != 0) return 0;
	}

	// test PKDBF2
	for (i = 0; i < _countof(pkcs5_vectors); i++)
	{
		pass  = pkcs5_vectors[i].password;
		salt  = pkcs5_vectors[i].salt;
		dklen = pkcs5_vectors[i].dklen;

		sha512_pkcs5_2(pkcs5_vectors[i].i_count, pass, strlen(pass), salt, strlen(salt), dk, dklen);
		if (memcmp(dk, pkcs5_vectors[i].key, dklen) != 0) return 0;
	}

	// all tests passed
	return 1;
}