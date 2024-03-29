#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class Microwave
{
   enum struct DOOR_STATE_type { OPEN, CLOSED };
   enum struct LIGHT_STATE_type { ON, OFF };
   enum struct POWER_STATE_type { ON, OFF };
   enum struct COOKING_STATE_type { COOKING, IDLE };
   
   public:
      Microwave() : door_state(DOOR_STATE_type::CLOSED), light_state(LIGHT_STATE_type::OFF), power_state(POWER_STATE_type::OFF), cooking_state(COOKING_STATE_type::IDLE), timer(0)
      {
         std::cout << "MW CREATED!" << std::endl << std::endl;
      }

      void event_set_timer_in_seconds(unsigned int seconds)
      {
         if (power_is_on() && mw_is_idle())
         {
            std::cout << "EVENT SET TIMER TO " << seconds << " SECONDS" << std::endl;
            timer = seconds;
         }
      }

      void event_start_cooking()
      {
         if(power_is_on() && door_is_closed() && timer_is_set() && mw_is_idle())
         {
            std::cout << "EVENT START COOKING" << std::endl;
            start_timer_countdown();
         }
      }

      void event_open_door() 
      {
         if (door_is_closed())
         {
            std::cout << "EVENT OPEN DOOR" << std::endl;
            door_state = DOOR_STATE_type::OPEN;

            if(mw_is_cooking())
            {
               event_pause_cooking();
            }
         }

         check_light_conditions();
      }

      void event_close_door() 
      {
         if (door_is_open())
         {
            std::cout << "EVENT CLOSE DOOR" << std::endl;
            door_state = DOOR_STATE_type::CLOSED;
         }

         check_light_conditions();
      }

      void event_power_off()
      {
         if(power_is_on())
         {
            std::cout << "EVENT POWER OFF" << std::endl;
            light_state = LIGHT_STATE_type::OFF;
            power_state = POWER_STATE_type::OFF;
            cooking_state = COOKING_STATE_type::IDLE;
            timer = 0;
         }

         check_light_conditions();
      }

      void event_power_on()
      {
         if(power_is_off())
         {
            std::cout << "EVENT POWER ON" << std::endl;
            power_state = POWER_STATE_type::ON;
            cooking_state = COOKING_STATE_type::IDLE;
            timer = 0;
         }

         check_light_conditions();
      }

      void event_pause_cooking()
      {
         // needs threading
      }

      void event_resume_cooking()
      {
         // needs threading
      }

   private:
      DOOR_STATE_type door_state;
      LIGHT_STATE_type light_state;
      POWER_STATE_type power_state;
      COOKING_STATE_type cooking_state;

      unsigned int timer;

      bool light_is_off() const { return LIGHT_STATE_type::OFF == light_state; }
      bool door_is_open() const { return DOOR_STATE_type::OPEN == door_state; }
      bool door_is_closed() const { return DOOR_STATE_type::CLOSED == door_state; }
      bool power_is_on() const { return POWER_STATE_type::ON == power_state; }
      bool power_is_off() const { return POWER_STATE_type::OFF == power_state; }
      bool mw_is_cooking() const { return COOKING_STATE_type::COOKING == cooking_state; }
      bool mw_is_idle() const { return COOKING_STATE_type::IDLE == cooking_state; }
      bool timer_is_set() const { return timer > 0; }

      bool light_on_conditions_met()
      {
         return (power_is_on() && light_is_off() && (door_is_open() || mw_is_cooking()));
      }

      void check_light_conditions()
      {
         if(light_on_conditions_met())
         {
            turn_light_on();
         }
         else
         {
            turn_light_off();
         }
      }
      
      void turn_light_on()
      {
         std::cout << "--light is ON" << std::endl;
         light_state = LIGHT_STATE_type::ON;
      }

      void turn_light_off()
      {
         std::cout << "--light is OFF" << std::endl;
         light_state = LIGHT_STATE_type::OFF;
      }

      void cook_timer_completed()
      {
         cooking_state = COOKING_STATE_type::IDLE;
         timer = 0;
         check_light_conditions();
      }

      void start_timer_countdown() // needs threading & mutex
      {
         cooking_state = COOKING_STATE_type::COOKING;
         check_light_conditions();

         for(unsigned int cd = timer; cd != 0; --cd)
         {
            std::cout << "--" << cd << " light is ";
            if(LIGHT_STATE_type::ON == light_state)
            {
               std::cout << "ON" << std::endl;
            }
            else
            {
               std::cout << "OFF" << std::endl;
            }

            std::this_thread::sleep_for(1s);
         }

         cook_timer_completed();
      }
};

int main ()
{
   Microwave mw;   

   mw.event_power_on();

   mw.event_open_door();
   mw.event_close_door();

   mw.event_set_timer_in_seconds(3);
   mw.event_start_cooking();

   mw.event_open_door();
   mw.event_close_door();

   mw.event_power_off();

   mw.event_open_door();
   mw.event_power_on();
   mw.event_close_door();

   return 0;
}
