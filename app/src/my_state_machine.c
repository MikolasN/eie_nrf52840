/**
 * @file my_state_machine.c
 */
 #include "my_state_machine.h"
 
 #include <zephyr/kernel.h>
 #include <zephyr/smf.h>

 #include "LED.h"
 #include "BTN.h"


 // Function Prototypes
static void led_on_state_exit(void* o);
static enum smf_state_result led_on_state_run(void* o);
static void led_off_state_exit(void* o);
static enum smf_state_result led_off_state_run(void* o);

// Typedefs
enum led_state_machine_states {
    LED_ON_STATE,
    LED_OFF_STATE
};

typedef struct {
    // Context variable used by zephyr to track the state machine's state
    struct smf_ctx ctx;

    uint32_t events;
} led_state_object_t;

// Local Variables
static const struct smf_state led_states[] = {
    [LED_ON_STATE] = SMF_CREATE_STATE(NULL, led_on_state_run,
         led_on_state_exit, NULL, NULL),
    [LED_OFF_STATE] = SMF_CREATE_STATE(NULL, led_off_state_run, 
        led_off_state_exit, NULL, NULL)
};

static led_state_object_t led_state_object;

// Functions 
 void state_machine_init() {
    led_state_object.events = 0;
    smf_set_initial(SMF_CTX(&led_state_object), &led_states[LED_ON_STATE]);
 }

 int state_machine_run() {
    led_state_object.events = k_event_wait_safe(&btn_event, BTN_ANY_EVENT, false, K_FOREVER);
    return smf_run_state(SMF_CTX(&led_state_object));
 }

static void led_on_state_exit(void* o) {
    LED_set(LED0, LED_ON);
}

static enum smf_state_result led_on_state_run(void* o) {
    if (led_state_object.events & BTN0_EVENT) {
        smf_set_state(SMF_CTX(&led_state_object), &led_states[LED_OFF_STATE]);
    }
    return SMF_EVENT_HANDLED;
 }

 static void led_off_state_exit(void* o) {
    LED_set(LED0, LED_OFF);
 }

 static enum smf_state_result led_off_state_run(void* o) {
    if (led_state_object.events & BTN1_EVENT) {
        smf_set_state(SMF_CTX(&led_state_object), &led_states[LED_ON_STATE]);
    }
    return SMF_EVENT_HANDLED;
 }