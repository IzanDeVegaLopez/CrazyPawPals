namespace ev_sys {

	struct event_info {
		int irrelevant_test_data;
	};

	delegate void Del(int, float);

	//Inherit from event_receiver in order to be able to consume events
	class event_receiver {
	public:
		virtual void handler(event_info&) = 0;
	};
		
	//To create new functions inherit from event_receiver
	//and override handler


	interface struct event_triggerer_interface {
	public:
		event Del^ E;
		void fire_event(event_info&);
	};

	//Obligatoriamente ha de crearse con gcnew por ser "ref"
	ref class event_source : public event_triggerer_interface {
		virtual event Del^ E;
		virtual void fire_event(event_info& e) {
			E(e);
		}
		void suscribe(event_receiver er) {
			E += gcnew Del(er, &event_receiver::handler);
		}
		void desuscribe(event_receiver er) {
			E -= gcnew Del(er, &event_receiver::handler);
		}
	};
}


//Case of Use
//Uncomment to see better
/*
int main() {
	//elements created by gcnew 
	//are automatically garbage-collected
	//aka they must not be deleted manually
	
	//create event triggerer (^ means that it will be created by gcnew)
	//Store as many event triggerers in the class that will fire them
	event_triggerer_interface^ _eti = gcnew event_source();
	//create event receiver
	event_receiver^ _er = new event_receiver();

	//suscribe
	_eti->E += gcnew Del(_er, &event_receiver::handler);
	//call the vent
	_eti->fire_event(event_info{ 1 });
	//desuscribe
	_eti->E -= gcnew Del(_er, &event_receiver::handler);
}
*/