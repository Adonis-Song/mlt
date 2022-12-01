#include <stdio.h>
#include <unistd.h>
#include <framework/mlt.h>
#if (defined(__APPLE__) || defined(_WIN32) || defined(HAVE_SDL2)) && !defined(MELT_NOSDL)
#include <SDL.h>
#endif

#if defined(SDL_MAJOR_VERSION)

static void event_handling( mlt_producer producer, mlt_consumer consumer )
{
	SDL_Event event;

	while ( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
			case SDL_QUIT:
				mlt_properties_set_int( MLT_PRODUCER_PROPERTIES( producer ), "done", 1 );
				break;

			case SDL_KEYDOWN:
#if SDL_MAJOR_VERSION == 2
				break;

			case SDL_WINDOWEVENT:
				if ( mlt_properties_get( MLT_CONSUMER_PROPERTIES(consumer), "mlt_service" ) &&
					 !strcmp( "sdl2", mlt_properties_get( MLT_CONSUMER_PROPERTIES(consumer), "mlt_service" ) ) )
				if ( event.window.event == SDL_WINDOWEVENT_RESIZED ||
					 event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
				{
					mlt_properties_set_int( MLT_CONSUMER_PROPERTIES(consumer),
						"window_width", event.window.data1 );
					mlt_properties_set_int( MLT_CONSUMER_PROPERTIES(consumer),
						"window_height", event.window.data2 );
				}
				break;
#else
				if ( event.key.keysym.unicode < 0x80 && event.key.keysym.unicode > 0 )
				{
					char keyboard[ 2 ] = { event.key.keysym.unicode, 0 };
					transport_action( producer, keyboard );
				}
				break;
#endif
		}
	}
}

#endif

static mlt_filter create_filter( mlt_profile profile, mlt_producer producer, char *effect)
{
	mlt_filter filter = NULL;
	int i = 0;
	int exists = 0;
	char *id = strdup( effect );
	char *arg = strchr( id, ':' );
	if ( arg != NULL )
		*arg ++ = '\0';

	for ( i = 0; ( filter = mlt_service_filter( MLT_PRODUCER_SERVICE(producer), i ) ) != NULL; i ++ )
	{
		// Check if this filter already exists
		char* filter_id = mlt_properties_get( MLT_FILTER_PROPERTIES(filter), "mlt_service");
		if ( filter_id && strcmp( id, filter_id ) == 0 )
		{
			exists = 1;
			break;
		}
		else if ( mlt_properties_get_int( MLT_FILTER_PROPERTIES( filter ), "_loader") == 0 )
		{
			// Stop at the first non-loader filter. This will be the insertion point for the new filter.
			break;
		}
	}

	if ( !exists )
	{
		filter = mlt_factory_filter( profile, id, arg );
		if ( filter )
		{
			mlt_properties_set_int( MLT_FILTER_PROPERTIES( filter ), "_loader", 1 );
			mlt_producer_attach( producer, filter );
			int last_filter_index = mlt_service_filter_count( MLT_PRODUCER_SERVICE(producer) ) - 1;
			if ( i != last_filter_index )
			{
				// Move the filter to be before any non-loader filters;
				mlt_service_move_filter( MLT_PRODUCER_SERVICE(producer), last_filter_index, i );
			}
			mlt_filter_close( filter );
		}
	}
	free( id );
	return filter;
}

mlt_producer create_playlist( int argc, char **argv, mlt_profile profile )
{
    // We're creating a playlist here
    mlt_playlist playlist = mlt_playlist_init( );

    // We need the playlist properties to ensure clean up
    mlt_properties properties = mlt_playlist_properties( playlist );

    // Loop through each of the arguments
    int i = 0;
    for ( i = 1; i < argc; i ++ )
    {
        // Create the producer
        mlt_producer producer = mlt_factory_producer( profile, NULL, argv[ i ] );
//		mlt_profile_from_producer(profile, producer);
//		mlt_producer_close(producer);
//		producer = mlt_factory_producer( profile, "avformat" , argv[i] );

//		create_filter( profile, producer, "movit.convert" );
//		create_filter( profile, producer, "avcolor_space");
//		create_filter( profile, producer, "audioconvert");
//		mlt_filter ass_filter = create_filter(profile, producer, "avfilter.ass");
//		mlt_properties_set(MLT_FILTER_PROPERTIES(ass_filter), "av.filename", "/Users/apple/Documents/mediaframework/profile-test/1/1.ass");
//		mlt_properties_set(MLT_FILTER_PROPERTIES(ass_filter), "av.fontsdir", "/Users/apple/Documents/mediaframework/profile-test/font/");

        // Add it to the playlist
        mlt_playlist_append( playlist, producer );

        // Close the producer (see below)
        mlt_producer_close( producer );
    }

    // Return the playlist as a producer
    return mlt_playlist_producer( playlist );
}

mlt_producer create_tracks( int argc, char **argv, mlt_profile profile)
{
    // Create the tractor
    mlt_tractor tractor = mlt_tractor_new( );

    // Obtain the field
    mlt_field field = mlt_tractor_field( tractor );

    // Obtain the multitrack
    mlt_multitrack multitrack = mlt_tractor_multitrack( tractor );

    // Create track 0
    mlt_producer track0 = create_playlist( argc, argv, profile );

	// Create a composite transition
    mlt_transition transition = mlt_factory_transition( profile, "composite", "50%/50%:50%x50%" );

    // Create the watermark track - note we NEED loader for scaling here
    // mlt_producer producer = mlt_factory_producer( profile, "loader","pixbuf:~/Downloads/demo.jpeg");
	mlt_producer producer = mlt_factory_producer( profile, "qimage", "/Users/apple/Downloads/demo1.jpeg" );

    // Get the length of track0
    mlt_position length = mlt_producer_get_playtime( track0 );

	mlt_playlist playlist = mlt_playlist_init();
	mlt_playlist_append_io(playlist, producer, 0, length - 1);
	mlt_producer_close(producer);
	mlt_producer track1 = mlt_playlist_producer(playlist);


    // Set the properties of track1
    mlt_properties properties = mlt_producer_properties( track1 );
    // mlt_properties_set( properties, "text", "Hello\nWorld" );
	// mlt_properties_set(properties, "resource", "~/Downloads/demo.jpeg");
	// mlt_properties_set(properties, "family", "/Users/apple/Downloads/hvizard-fonts-0805/FandolHei/FandolHei-Regular.otf");
    // mlt_properties_set_position( properties, "in", 0 );
    // mlt_properties_set_position( properties, "out", length - 1 );
    mlt_properties_set_position( properties, "length", length );
    mlt_properties_set_int( properties, "a_track", 0 );
    mlt_properties_set_int( properties, "b_track", 1 );

    // Now set the properties on the transition
    properties = mlt_transition_properties( transition );
    mlt_properties_set_position( properties, "in", 0 );
    mlt_properties_set_position( properties, "out", length - 1 );

    // Add our tracks to the multitrack
    mlt_multitrack_connect( multitrack, track0, 0 );
    mlt_multitrack_connect( multitrack, track1, 1 );

    // Now plant the transition
    mlt_field_plant_transition( field, transition, 0, 1 );

    // Close our references
    mlt_producer_close( track0 );
    mlt_producer_close( track1 );
    mlt_transition_close( transition );

    // Return the tractor
    return mlt_tractor_producer( tractor );
}

int main( int argc, char *argv[] )
{
    // Initialise the factory
    if ( mlt_factory_init( NULL ) != 0 )
    {

		mlt_profile profile = mlt_profile_init(NULL);

        // Create via the default producer
        // mlt_producer world = mlt_factory_producer(profile, "avformat", argv[1]);
		// mlt_profile_from_producer(profile, world);
		// mlt_producer_close(world);
		// world = mlt_factory_producer(profile, "avformat", argv[1]);
		// create_filter( profile, world, "movit.convert" );
		// create_filter( profile, world, "avcolor_space");
		// create_filter( profile, world, "audioconvert");

		mlt_producer world = create_tracks(argc, argv, profile);
		// mlt_properties_save(mlt_producer_properties(world), "./song.txt");

		// Create the default consumer
        // mlt_consumer hello = mlt_factory_consumer(profile, "avformat", "./song.mp4");
		mlt_consumer hello = mlt_factory_consumer(profile, NULL, NULL);
		// mlt_consumer hello = mlt_factory_consumer(profile, NULL, NULL);

        // Connect the producer to the consumer
        mlt_consumer_connect( hello, mlt_producer_service( world ) );

        // Start the consumer
        mlt_consumer_start( hello );

		mlt_properties properties = mlt_consumer_properties(world);

        // Wait for the consumer to terminate
        while( !mlt_consumer_is_stopped( hello ) && mlt_properties_get_int( properties, "done" ) == 0 )
        {
			event_handling( world, hello );
			usleep(200000);
		}

        // Close the consumer
        mlt_consumer_close( hello );

        // Close the producer
        mlt_producer_close( world );

        // Close the factory
        mlt_factory_close( );
    }
    else
    {
        // Report an error during initialisation
        fprintf( stderr, "Unable to locate factory modules\n" );
    }

    // End of program
    return 0;
}
