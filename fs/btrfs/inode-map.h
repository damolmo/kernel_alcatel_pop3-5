#ifndef __BTRFS_INODE_MAP
#define __BTRFS_INODE_MAP

void btrfs_init_free_ino_ctl(struct btrfs_root *root);
void btrfs_unpin_free_ino(struct btrfs_root *root);
void btrfs_return_ino(struct btrfs_root *root, u64 objectid);
int btrfs_find_free_ino(struct btrfs_root *root, u64 *objectid);
int btrfs_save_ino_cache(struct btrfs_root *root,
			 struct btrfs_trans_handle *trans);

int btrfs_find_free_objectid(struct btrfs_root *root, u64 *objectid);
<<<<<<< HEAD
=======
int btrfs_find_highest_objectid(struct btrfs_root *root, u64 *objectid);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

#endif
