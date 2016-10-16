#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/ImageObject>
#include <Lib/IPLab.h>
#include <Lib/IPLabList.h>
#include <Lib/Importer.h>
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
    ReadIPLab::IPLabList ipl_list( dirname );
    ReadIPLab::IPLab ipl = ipl_list.load( tindex );

    // Import and register image object
    kvs::ImageObject* image = ReadIPLab::ImportImageObject( ipl, sindex );
    screen.registerObject( image );
    screen.show();

    return app.run();
}
