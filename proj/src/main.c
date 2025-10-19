#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "Controller/Game/GameController.h"
#include "include.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)() {

  int ipc_status;
  message msg;
  int r;

  uint8_t bit_no_timer;
  uint8_t bit_no_keyboard;
  uint8_t bit_no_mouse;

  if (vg_map(VBE_1152) != 0)
    return 1;

  if (VG_init(VBE_1152) != 0)
    return 1;

  if (timer_subscribe_int(&bit_no_timer) != 0)
    return 1;
  if (keyboard_subscribe_int(&bit_no_keyboard) != 0)
    return 1;
  if (mouse_subscribe_int(&bit_no_mouse) != 0)
    return 1;
  if (mouse_data_reporting(0xF4) != 0)
    return 1;

  gamecontroller_init();
  uint16_t defaultMode = 0x14C;
  setup_sprites(defaultMode);

  while (gamestate != EXIT_STATE) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                       /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(bit_no_mouse)) { /* subscribed interrupt */
            update_Mouse();
          }

          if (msg.m_notify.interrupts & BIT(bit_no_keyboard)) { /* subscribed interrupt */
            update_Keyboard();
          }

          if (msg.m_notify.interrupts & BIT(bit_no_timer)) { /* subscribed interrupt */
            update_Timer();
          }

          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  gamecontroller_exit();

  if (mouse_data_reporting(0xF5) != 0)
    return 1;

  if (timer_unsubscribe_int() != 0)
  return 1;

  if (keyboard_unsubscribe_int() != 0)
    return 1;
  if (mouse_unsubscribe_int() != 0)
  return 1;

  if (vg_exit() != 0)
    return 1;


  return 0;
}
