#include <iostream>
#include <string>
#include <vector>

#include "fun.hpp"

using namespace std;
using namespace app;

#include <cassert>
#include <boost/lexical_cast.hpp>

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <QApplication>
#include "main_window.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace boost::posix_time;
using namespace boost::gregorian;

void check_time(time_t tsec)
{
    ptime pt = from_time_t(tsec);
    date d = pt.date();
    time_duration t = pt.time_of_day();
    cout << "date: " << d.year() << ":" << d.month().as_number() << ":" << d.day() << ":" << d.day_of_week() << ":" << d.end_of_month() << endl;
    cout << "time: " << t.hours() << ":" << t.minutes() << ":" << t.seconds() << endl;
}

void check_file(path& p)
{
    assert(is_regular_file(p));

    cout << "size: " << file_size(p) << endl;
    time_t mod_time = last_write_time(p);
    cout << "mod time: " << mod_time << endl;
    cout << "mod time text: " << ctime(&mod_time) << endl;

    check_time(mod_time);
}

void check_dir(path& p)
{
    assert(is_directory(p));
    cout << endl << "list dir: ------" << p << endl;
    for (directory_entry& e : directory_iterator(p)) {
        path e_path = e.path();
        file_status e_stat = e.status();
        cout << e_path << endl;
        cout << "regular_file: " << is_regular_file(e_stat) << endl;
        cout << "dir: " << is_directory(e_stat) << endl;
        if (is_directory(e_path)) {
            try {
                check_dir(e_path);
            } catch (const filesystem_error& ex) {
                cout << "Error: " << ex.what() << endl;
            }
        } else if (is_regular_file(e_path)) {
            check_file(e_path);
        }
    }
}

void check_path(path& p)
{
    cout << "path: " << p << endl;
    if (exists(p)) {
        cout << p << " exists" << endl;
    } else {
        cout << p << " not exists" << endl;
    }

    if (is_regular_file(p)) {
        cout << "file: " << p << ", size: " << file_size(p) << endl;
    } else {
        cout << p << " not file" << endl;
    }

    if (is_directory(p)) {
        cout << "dir: " << p << endl;
        check_dir(p);
    } else {
        cout << p << " not dir" << endl;
    }
}

void test_boost_fs()
{
    path p1{"C:\\mytest"};
    check_path(p1);

    path p2 = current_path();
    check_path(p2);

    path p3{ "C:\\lxu\\sdev\\bsd_prj\\new\\prj\\sqlite1\\src\\runsql\\main.cpp" };
    check_path(p3);
}

void test_boost()
{
    cout << "test boost base" << endl;

    string str = "12345";
    int i;
    try {   
        i = boost::lexical_cast<int>(str);
        cout << "Value: " << i << endl;
    } catch( const boost::bad_lexical_cast & ) {
        //unable to convert
        cerr << "Error: failed to convert";
    }
}

void test_stl()
{
	vector<int> l = {10, 20, 30};

	for (auto i : l) {
//		string txt = run_fun(i);
//		cout << txt;
		cout << run_fun(i) << endl;
	}	
}

int main(int argc, char* argv[])
{
	test_stl();
	test_boost();
	test_boost_fs();
	
    QApplication app(argc, argv);
    MainWindow mainWindow;

    mainWindow.show();
    return app.exec();    
}

