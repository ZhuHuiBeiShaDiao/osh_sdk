#pragma once

class c_menu {
private:
	std::shared_ptr< OSHGui::Form > m_form;
public:
	void init( );

	bool test;
};

class c_main_form : public OSHGui::Form {
private:
	void init_component( );
public:
	c_main_form( ) {
		init_component( );
	}
};

extern c_menu g_menu;