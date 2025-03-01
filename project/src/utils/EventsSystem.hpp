#include <stdio.h>

namespace ev_sys {

	struct event_info {
		int irrelevant_test_data = 0;
	};

	[event_source(_NATIVE_NULLPTR_SUPPORTED)]
	class event_source {
	public:
		__event void my_event(event_info& ei);
	};

	[event_receiver(_NATIVE_NULLPTR_SUPPORTED)]
	class event_receiver {
	public:
		virtual void handler(event_info&) = 0;
		void hook_event(event_source* es) {
			__hook(&event_source::my_event, es, &event_receiver::handler);
		}
		void unhook_event(event_source* es) {
			__unhook(&event_source::my_event, es, &event_receiver::handler);
		}
	};
}


//Case of Use
/*
int main() {
	ev_sys::event_source source;
	ev_sys::event_receiver receiver;

	//suscribe to event
	receiver.hook_event(&source);
	//invoke event
	ev_sys::event_info ev = ev_sys::event_info();
	ev.irrelevant_test_data = 0;
	__raise source.my_event(ev);
	//unsuscribe to event
	receiver.unhook_event(&source);
}
*/