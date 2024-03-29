#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class Microwave
{
   public:
      Microwave() : door_is_open(false), light_is_on(false), power_is_on(true), is_idle(true), timer(0)
      {}

      bool the_door_is_open() { return door_is_open; }
      bool the_door_is_closed() { return !door_is_open; }

      bool the_light_is_on() { return light_is_on; }
      bool the_light_is_off() { return !light_is_on; }

      bool the_power_is_on() { return power_is_on; }
      bool the_power_is_off() { return !power_is_on; }

      bool is_cooking() { return !is_idle; }
      bool not_cooking() { return is_idle; }

      bool timer_is_set() { return timer > 0;}

      void set_timer_in_seconds(unsigned int seconds)
      {
         if (the_door_is_closed())
         {
            std::cout << "SET TIMER TO " << seconds << " SECONDS" << std::endl;
            timer = seconds;
         }
      }

      void event_start_cooking()
      {
         if(the_power_is_on() &&
            the_door_is_closed() &&
            timer_is_set())
         {
               is_idle = false;

               turn_light_on();
               timer_countdown();

               is_idle = true;
               timer = 0;
               turn_light_off();
         }
      }

      void event_pause_cooking()
      {
         // needs threading
      }

      void event_resume_cooking()
      {
         // needs threading
      }

      void event_open_door() 
      {
         if (the_door_is_closed())
         {
            std::cout << "OPEN DOOR" << std::endl;

            door_is_open = true;

            turn_light_on();

            if(is_cooking())
            {
               event_pause_cooking();
            }
         }
      }

      void event_close_door() 
      {
         if (the_door_is_open())
         {
            std::cout << "CLOSE DOOR" << std::endl;

            door_is_open = false;

            turn_light_off();
         }
      }

      void event_power_off()
      {
         std::cout << "POWER OFF" << std::endl;

         light_is_on = false;
         power_is_on = false;
         is_idle = true;
         timer = 0;

         turn_light_off();
      }

      void event_power_on()
      {
         std::cout << "POWER ON" << std::endl;
         power_is_on = true;
         timer = 0;
      }

   private:
      bool door_is_open;
      bool light_is_on;
      bool power_is_on;
      bool is_idle;

      unsigned int timer;

      bool light_on_conditions_met()
      {
         return (the_power_is_on() && (the_door_is_open() || is_cooking()));
      }

      void turn_light_off()
      {
         if(!light_on_conditions_met() && light_is_on)
         {
            std::cout << "--LIGHT OFF" << std::endl;

            light_is_on = false;
         }
      }

      void turn_light_on()
      {
         if(light_on_conditions_met() && !light_is_on)
         {
            std::cout << "--LIGHT ON" << std::endl;

            light_is_on = true;
         }
      }

      void timer_countdown()
      {
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
      }
};

int main ()
{
   Microwave mw;   

   mw.event_power_on();

   mw.event_open_door();
   mw.event_close_door();

   mw.set_timer_in_seconds(10);
   mw.event_start_cooking();

   mw.event_open_door();
   mw.event_close_door();

   mw.event_power_off();

   return 0;
}
