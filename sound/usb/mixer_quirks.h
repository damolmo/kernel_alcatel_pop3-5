#ifndef SND_USB_MIXER_QUIRKS_H
#define SND_USB_MIXER_QUIRKS_H

int snd_usb_mixer_apply_create_quirk(struct usb_mixer_interface *mixer);

void snd_emuusb_set_samplerate(struct snd_usb_audio *chip,
			       unsigned char samplerate_id);

void snd_usb_mixer_rc_memory_change(struct usb_mixer_interface *mixer,
				    int unitid);

<<<<<<< HEAD
=======
void snd_usb_mixer_fu_apply_quirk(struct usb_mixer_interface *mixer,
				  struct usb_mixer_elem_info *cval, int unitid,
				  struct snd_kcontrol *kctl);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif /* SND_USB_MIXER_QUIRKS_H */

