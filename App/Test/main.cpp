#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/ImageObject>
#include <kvs/ImageImporter>
#include <kvs/IPLab>
#include <kvs/IPLabList>
#include <iostream>


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    // Input parameters
    const std::string dirname( argv[1] );
    const size_t tindex = 0;
    const size_t sindex = 20;
    std::cout << "Input directory: " << dirname << std::endl;
    std::cout << "Time index: " << tindex << std::endl;
    std::cout << "Slice index: " << sindex << std::endl;

    // Read IPLab image dataset
    kvs::IPLabList ipl_list( dirname );
    kvs::IPLab ipl = ipl_list.load( tindex );
    ipl.setImportingFrameIndex( sindex );

    // Import and register image object
    kvs::ImageObject* image = new kvs::ImageImporter( &ipl );
    screen.registerObject( image );
    screen.show();

    return app.run();
}
