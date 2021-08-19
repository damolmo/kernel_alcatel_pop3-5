#ifndef _CRYPTO_SHA256_GLUE_H
#define _CRYPTO_SHA256_GLUE_H

#include <linux/crypto.h>
<<<<<<< HEAD

extern struct shash_alg sha256_neon_algs[2];

int crypto_sha256_arm_update(struct shash_desc *desc, const u8 *data,
			     unsigned int len);

int crypto_sha256_arm_finup(struct shash_desc *desc, const u8 *data,
			    unsigned int len, u8 *hash);
=======
#include <crypto/sha.h>

extern struct shash_alg sha256_neon_algs[2];

extern int sha256_init(struct shash_desc *desc);

extern int sha224_init(struct shash_desc *desc);

extern int __sha256_update(struct shash_desc *desc, const u8 *data,
			   unsigned int len, unsigned int partial);

extern int sha256_update(struct shash_desc *desc, const u8 *data,
			 unsigned int len);

extern int sha256_export(struct shash_desc *desc, void *out);

extern int sha256_import(struct shash_desc *desc, const void *in);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

#endif /* _CRYPTO_SHA256_GLUE_H */
