#include <iostream>
#include <cstdio>
#include <vector>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/ValueArray>
#include <kvs/LineObject>
#include <kvs/ImageObject>
#include <kvs/ExternalFaces>
#include <ReadBDML/Lib/BDMLData.h>
#include <ReadIPLab/Lib/IPLab.h>
#include <ReadIPLab/Lib/IPLabList.h>
#include <kvs/KeyPressEventListener>

#include <kvs/glut/Label>
#include <kvs/PaintEventListener>

kvs::ImageObject* GetImageObject( const ReadIPLab::IPLab& ipl, const size_t sindex )
{
  kvs::ImageObject* object = new kvs::ImageObject();
  object->setName( "Image" );
  object->setSize( ipl.width(), ipl.height() );
  object->setPixels( ipl.data(sindex), kvs::ImageObject::Gray8 );
  return object;
}

kvs::ImageObject* GetImageObjectWithEdge( kvs::ImageObject* image, ReadBDML::BDMLData& bdml, const size_t sindex, const size_t tindex )
{
  kvs::ValueArray<kvs::UInt8> pixels = image->pixels();

  const size_t ncomponents = bdml.data().components().size();
  for ( size_t cindex = 0; cindex < ncomponents; cindex++ )
    {
      const ReadBDML::bdml::ComponentTag& component = bdml.data().component(cindex);
	  
      if ( component.time() == tindex )
	{
	  const size_t nsequences = component.measurement().line().xyzSequences().size();
	  for ( size_t z = 0; z < nsequences; z++ ){
	    const ReadBDML::bdml::XYZSequenceTag& sequence = component.measurement().line().xyzSequence(z);
	    if ( sequence.xyz().size() == 0 ) { continue; }
	    else if( sequence.xyz()[2] == sindex ){
	      for(int i = 0;i<(int)sequence.xyz().size()/3;i++){
		int x = sequence.xyz()[i*3];
		int y = sequence.xyz()[i*3+1];
		pixels[ image->width() * y + x ] = 255;
	      }
	    }
	  }
	}
    }
  
  kvs::ImageObject* object = new kvs::ImageObject();
  object->setSize( image->width(), image->height() );
  object->setPixels( pixels, kvs::ImageObject::Gray8 );
  return object;
}

class PaintEvent : public kvs::PaintEventListener
{
  kvs::glut::Label& m_label;
  ReadBDML::BDMLData& m_bdml; 
  int& m_tindex; // 時間インデックス
  int& m_sindex; // スライスインデックス
  
public:

  PaintEvent( kvs::glut::Label& label,
	      ReadBDML::BDMLData& bdml,
	      int& tindex,
	      int& sindex ):
    m_label( label ),
    m_bdml( bdml ),
    m_tindex( tindex ),
    m_sindex( sindex )
  {
 
    m_label.setMargin( 10 );

  }
    

  void update()
  {
    m_label.setText("Time index: %d", m_tindex );
    m_label.addText("Slice index: %d", m_sindex );
    //add
    const size_t ncomponents = m_bdml.data().components().size();
    for ( size_t cindex = 0; cindex < ncomponents; cindex++ )
      {
	const ReadBDML::bdml::ComponentTag& component = m_bdml.data().component(cindex);
	  
	if ( component.time() == m_tindex )
	  {
	    	    m_label.addText("");
	    m_label.addText("component[%d]:", cindex );
  
	    m_label.addText("componentID:%s", component.componentID().c_str() );
	    m_label.addText("componentName:%s", component.componentName().c_str() );
	    m_label.addText("prevID:%s", component.prevID().c_str() );
	   
	  }
    
      }
    //
    screen()->redraw();
  }
};


class KeyPress : public kvs::KeyPressEventListener
{ 
  ReadIPLab::IPLabList& m_ipl_list; // IPLabデータディレクトリ
  ReadIPLab::IPLab& m_ipl; // 指定されたIPLabデータ
  ReadBDML::BDMLData& m_bdml;  
  int& m_tindex; // 時間インデックス
  int& m_sindex; // スライスインデックス

public:

  KeyPress(
	   ReadIPLab::IPLabList& ipl_list,
	   ReadIPLab::IPLab& ipl,
	   ReadBDML::BDMLData& bdml,//bdml
	   int& tindex,
	   int& sindex ):
    m_ipl_list( ipl_list ),
    m_ipl( ipl ),
    m_bdml( bdml ),
    m_tindex( tindex ),
    m_sindex( sindex )
  {
  }
  
    void update( kvs::KeyEvent* event )
    {
        switch ( event->key() )
        {
        case kvs::Key::Up: // 上矢印キーが押下された時の処理
        {
            // スライスインデックスをインクリメントする。
            // ただし、最大スライス数を超えないようにする。
            const int max_value = m_ipl.numberOfFrames() - 1;
            m_sindex = kvs::Math::Min( ++m_sindex, max_value );
            break;
        }
        case kvs::Key::Down: // 下矢印キーが押下された時の処理
        {
            // スライスインデックスをデクリメントする。
            // ただし、最小スライス数（ゼロ）を下回らないようにする。
            const int min_value = 0;
            m_sindex = kvs::Math::Max( --m_sindex, min_value );
            break;
        }
        case kvs::Key::Right: // 右矢印キーが押下された時の処理
        {
	  // 時間インデックスをインクリメントする。
	  // ただし、最大時間数（ファイル数）を超えないようにする。
	  const int max_value = m_ipl_list.filenames().size() - 1;
	  m_tindex = kvs::Math::Min( ++m_tindex, max_value );
	  m_ipl = m_ipl_list.load( m_tindex );
	  break;
        }
        case kvs::Key::Left: // 左矢印キーが押下された時の処理
        {
            // 時間インデックスをデクリメントする。
            // ただし、最小時間数（ゼロ）を下回らないようにする。
            const int min_value = 0;
            m_tindex = kvs::Math::Max( --m_tindex, min_value );
            m_ipl = m_ipl_list.load( m_tindex );
            break;
        }
        default: break;
        }

        // スクリーンに登録されている画像オブジェクトを更新（削除＆再登録）する。
        scene()->removeObject( "Image" ); // 登録されている画像オブジェクトの削除
	kvs::ImageObject* image = GetImageObject( m_ipl, m_sindex );
	kvs::ImageObject* object = GetImageObjectWithEdge( image, m_bdml, m_sindex, m_tindex );
	delete image;
        scene()->registerObject( object ); // 画像オブジェクトの取得と登録
	//scene()->redraw();
    }
};

int main( int argc, char** argv )
{
  kvs::glut::Application app( argc, argv );
  kvs::glut::Screen screen( &app );
  screen.show();
  //Image読み込み
  ReadBDML::BDMLData bdml(argv[1]);
  ReadIPLab::IPLabList ipl_list;
  ipl_list.read( argv[2] );
  int tindex =30; //time指定
  ReadIPLab::IPLab ipl = ipl_list.load( tindex );
  int sindex =30; //z指定
      
  kvs::ImageObject* image = GetImageObject( ipl, sindex );
  kvs::ImageObject* object = GetImageObjectWithEdge( image, bdml, sindex, tindex );
  
  delete image;
  screen.registerObject( object );

  // キーボード処理イベントを定義しスクリーンに登録する。【追加】
  KeyPress key_press( ipl_list, ipl, bdml, tindex, sindex );
  screen.addEvent( &key_press );

  kvs::glut::Label label( &screen );
  label.setTextColor( kvs::RGBColor::White() );
  label.show();
  PaintEvent paint_event( label, bdml, tindex, sindex );

  screen.addEvent( &paint_event );

  
  //screen.show();
  return app.run();
}
