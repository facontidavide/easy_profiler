/************************************************************************
* file name         : main_window.h
* ----------------- :
* creation time     : 2016/06/26
* copyright         : (c) 2016 Victor Zarubkin
* author            : Victor Zarubkin
* email             : v.s.zarubkin@gmail.com
* ----------------- :
* description       : The file contains declaration of MainWindow for easy_profiler GUI.
* ----------------- :
* change log        : * 2016/06/26 Victor Zarubkin: initial commit.
*                   : *
* ----------------- :
* license           : TODO: add license text
************************************************************************/

#ifndef EASY_PROFILER_GUI__MAIN_WINDOW__H
#define EASY_PROFILER_GUI__MAIN_WINDOW__H

#include <string>
#include <thread>
#include <atomic>
#include <QMainWindow>
#include <QTimer>
#include "profiler/reader.h"

//////////////////////////////////////////////////////////////////////////

class QDockWidget;

//////////////////////////////////////////////////////////////////////////

class EasyFileReader final
{
    ::profiler::SerializedData   m_serializedData; ///< 
    ::profiler::thread_blocks_tree_t m_blocksTree; ///< 
    ::std::string                      m_filename; ///< 
    ::std::thread                        m_thread; ///< 
    ::std::atomic_bool                    m_bDone; ///< 
    ::std::atomic<int>                 m_progress; ///< 
    ::std::atomic<unsigned int>            m_size; ///< 

public:

    EasyFileReader();
    ~EasyFileReader();

    bool done() const;
    int progress() const;
    unsigned int size() const;

    void load(const ::std::string& _filename);
    void interrupt();
    void get(::profiler::SerializedData& _data, ::profiler::thread_blocks_tree_t& _tree, ::std::string& _filename);

}; // END of class EasyFileReader.

//////////////////////////////////////////////////////////////////////////

class EasyMainWindow : public QMainWindow
{
    Q_OBJECT

protected:

    typedef EasyMainWindow This;
    typedef QMainWindow  Parent;

    ::std::string                    m_lastFile;
    QDockWidget*                   m_treeWidget;
    QDockWidget*                 m_graphicsView;
    class QProgressDialog*           m_progress;
    QTimer                        m_readerTimer;
    ::profiler::SerializedData m_serializedData;
    EasyFileReader                     m_reader;

public:

    EasyMainWindow();
    virtual ~EasyMainWindow();

    // Public virtual methods

    void closeEvent(QCloseEvent* close_event) override;

protected slots:

    void onOpenFileClicked(bool);
    void onReloadFileClicked(bool);
    void onExitClicked(bool);
    void onEncodingChanged(bool);
    void onDrawBordersChanged(bool);
    void onFileReaderTimeout();
    void onFileReaderCancel();

private:

    // Private non-virtual methods

    void loadFile(const std::string& filename);

    void loadSettings();
	void saveSettings();

}; // END of class EasyMainWindow.

//////////////////////////////////////////////////////////////////////////

#endif // EASY_PROFILER_GUI__MAIN_WINDOW__H
