#include "../includes.h"
#include "menu.h"

c_menu g_menu;

void c_menu::init( ) {
	m_form = std::static_pointer_cast< OSHGui::Form >( std::make_shared< c_main_form >( ) );
	m_form->SetFont( g_renderer->get_instance( )->GetDefaultFont( ) );

	g_renderer->get_instance( )->Run( m_form );

	g_renderer->get_instance( )->Enable( );
}

void c_main_form::init_component( ) {
	SetSize( OSHGui::Drawing::SizeI( 600, 400 ) );
	SetText( "osh-sdk" );

	bool var = false;

	OSHGui::CheckBox *c = new OSHGui::CheckBox(  );
	c->SetText( "Test" );
	c->SetChecked( g_menu.test );

	AddControl( c );

	// click event.
	c->GetClickEvent( ) += OSHGui::ClickEventHandler( [ & ]( Control *sender ) {
		g_menu.test = !g_menu.test;
	});
}