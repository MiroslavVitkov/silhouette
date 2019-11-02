#ifndef CMD_H_
#define CMD_H_


// In this file: a list of all high-level actions.


#include <memory>
#include <string>


namespace cmd
{


struct Base
{
    virtual void execute() = 0;
    virtual ~Base() = default;
};


struct NoOp : Base
{
    void execute() override {}
};


// camera -> detect pedestrians -> play video
struct CamDetectShow : Base
{
    void execute() override;
};


// Supervisely dataset -> detect pedestrians -> play video
struct DatDetectShow : Base
{
    void execute() override;
};


// Measure the accuracy of the OpenCV pretrained pedestrian detector
// over the Supervisely Persons Dataset.
// Supervisely dataset -> detect pedestrians -> measure accuracy
struct DatDetectSummarise : Base
{
    void execute() override;
};


}  // namespace cmd


#endif  // defined( CMD_H_ )
