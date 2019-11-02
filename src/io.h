#ifndef IO_H_
#define IO_H_


// In this file: input and output of pictures and videos.


#include "except.h"
#include "bitmap.h"

#include <opencv2/opencv.hpp>

#include <memory>
#include <string>
#include <vector>


namespace io
{


enum class Mode
{
    _colour,
    _grayscale,
};


struct FrameSource
{
    virtual FrameSource & operator>>( cv::Mat & frame ) = 0;
    virtual operator bool() const = 0;  // true if last operation was successful
    virtual cv::Size get_size() const = 0;
    virtual ~FrameSource() = default;
};


struct FrameSink
{
    virtual FrameSink & operator<<( const cv::Mat & frame ) = 0;
    virtual ~FrameSink() = default;
};


struct Camera : FrameSource
{
    enum class Id : int
    {
        // docs.opencv.org/3.2.0/d8/dfe/classcv_1_1VideoCapture.html#a5d5f5dacb77bbebdcbfb341e3d4355c1
        _first_usb_camera = 0
    };

    Camera( Mode = Mode::_colour, Id = Id::_first_usb_camera );
    operator bool() const override { return true; }
    cv::Size get_size() const override;
    Camera & operator>>( cv::Mat & frame ) override;

private:
    cv::VideoCapture _video_stream;
    const Mode _mode;
};


struct VideoReader : FrameSource
{
    VideoReader( const std::string & path );
    operator bool() const override;
    cv::Size get_size() const override;
    VideoReader & operator>>( cv::Mat & frame ) override;

private:
    cv::VideoCapture _video_stream;
    bool _good;
};


// A directory with one subdirectory per subject.
// Subdirectory names are the labels.
struct DirReader : FrameSource
{
    DirReader( const std::string & path
             , Mode mode = Mode::_colour
             , bool calc_size = false );
    DirReader( DirReader && );
    ~DirReader() override;

    operator bool() const override;
    DirReader & operator>>( cv::Mat & face ) override;
    cv::Size get_size() const override;
    const std::string & get_label() const;

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};


std::vector<DirReader> get_subdirs( const std::string & dataset_path
                                  , Mode mode = Mode::_colour
                                  , bool calc_size = false );

enum class Colour
{
    _green,
    _red,
};

void draw_rects( cv::Mat & frame, const std::vector<cv::Rect> & rects, Colour c = Colour::_green );

cv::Mat crop( const cv::Mat & frame, const cv::Rect & rect );
std::vector<cv::Mat> crop( const cv::Mat & frame
                         , const std::vector<cv::Rect> & rects );


struct SuperviselyReader : FrameSource
{
    SuperviselyReader( const std::string & path );
    ~SuperviselyReader() override;
    SuperviselyReader & operator>>( cv::Mat & frame ) override;
    virtual operator bool() const override;
    virtual cv::Size get_size() const override;
    std::vector< bitmap::Silhouette > get_last_silhouettes() const;

private:
    struct Impl;
    std::unique_ptr< Impl > _impl;
};


struct VideoPlayer : FrameSink
{
    VideoPlayer( const std::string & window_name = "" );
    VideoPlayer & operator<<( const cv::Mat & frame ) override;

private:
    const std::string _window_name;
};


struct VideoWriter : FrameSink
{
    // How to fit smaller frames to the video.
    enum class Fit{ _border };

    VideoWriter( const std::string & path
               , cv::Size
               , Fit fit_mode = Fit::_border );
    VideoWriter & operator<<( const cv::Mat & frame ) override;

private:
    cv::VideoWriter _video_stream;
    const cv::Size _size;
    const Fit _fit;
};


struct DirWriter : public FrameSink
{
    DirWriter( const std::string path );
    DirWriter & operator<<( const cv::Mat & frame ) override;

private:
    const std::string _path;
    long unsigned _frame_num;
};


}  // namespace io


#endif // defined( IO_H_ )
