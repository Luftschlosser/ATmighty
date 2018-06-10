/*
 * This headerfile defines for each interrupt-source whether it should be managed by ATmighty (true) or by the user (false).
 */

#ifndef CONFIG_INTERRUPTCONFIG_H_
#define CONFIG_INTERRUPTCONFIG_H_


//Timer0:
#define ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV0 true
#define ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0A true
#define ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF0B true

//Timer2:
#define ATMIGHTY_INTERRUPTCONFIG_MANAGE_TOV2 true
#define ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2A true
#define ATMIGHTY_INTERRUPTCONFIG_MANAGE_OCF2B true


#endif /* CONFIG_INTERRUPTCONFIG_H_ */
