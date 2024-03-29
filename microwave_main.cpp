#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class Microwave
{
   public:
      Microwave() : door_is_open(false), light_is_on(false), power_is_on(false), cooking(false), timer(0)
      {}

      void event_set_timer_in_seconds(unsigned int seconds)
      {
         if (the_power_is_on() && is_not_cooking())
         {
            std::cout << "SET TIMER TO " << seconds << " SECONDS" << std::endl;
            timer = seconds;
         }
      }

      void event_start_cooking()
      {
         if(the_power_is_on() && the_door_is_closed() && timer_is_set() && is_not_cooking())
         {
            std::cout << "START COOKING FOR " << timer << " SECONDS" << std::endl;
            start_timer_countdown();
         }
      }

      void event_open_door() 
      {
         if (the_door_is_closed())
         {
            std::cout << "OPEN DOOR" << std::endl;
            door_is_open = true;

            if(is_cooking())
            {
               event_pause_cooking();
            }
         }

         check_light_conditions();
      }

      void event_close_door() 
      {
         if (the_door_is_open())
         {
            std::cout << "CLOSE DOOR" << std::endl;
            door_is_open = false;
         }

         check_light_conditions();
      }

      void event_power_off()
      {
         if(the_power_is_on())
         {
            std::cout << "POWER OFF" << std::endl;
            light_is_on = false;
            power_is_on = false;
            cooking = false;
            timer = 0;
         }

         check_light_conditions();
      }

      void event_power_on()
      {
         if(the_power_is_off())
         {
            std::cout << "POWER ON" << std::endl;
            power_is_on = true;
            cooking = false;
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
      bool door_is_open;
      bool light_is_on;
      bool power_is_on;
      bool cooking;

      unsigned int timer;

      bool the_light_is_off() { return !light_is_on; }
      bool the_door_is_open() { return door_is_open; }
      bool the_door_is_closed() { return !door_is_open; }
      bool the_power_is_on() { return power_is_on; }
      bool the_power_is_off() { return !power_is_on; }
      bool is_cooking() { return cooking; }
      bool is_not_cooking() { return !cooking; }
      bool timer_is_set() { return timer > 0; }

      bool light_on_conditions_met()
      {
         return (the_power_is_on() && the_light_is_off() && (the_door_is_open() || is_cooking()));
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
         std::cout << "--LIGHT ON" << std::endl;
         light_is_on = true;
      }

      void turn_light_off()
      {
         std::cout << "--LIGHT OFF" << std::endl;
         light_is_on = false;
      }

      void cook_timer_completed()
      {
         cooking = false;
         timer = 0;
         check_light_conditions();
      }

      void start_timer_countdown()
      {
         cooking = true;
         check_light_conditions();

         for(int cd = timer; cd != 0; --cd)
         {
            std::cout << "--" << cd << " LIGHT ";
            if(light_is_on)
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
