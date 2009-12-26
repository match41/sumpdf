#include <QApplication>
#include <QWidget>

#include <libpdfdoc.hh>
#include <Doc.hh>

int main( int argc, char **argv )
{
    QApplication app(argc, argv);

	pdf::Doc *doc = pdf::CreateDoc( ) ;
	delete doc ;

    QWidget *window = new QWidget();
    window->resize(320, 240);
    window->show();

    return app.exec( ) ;
}
