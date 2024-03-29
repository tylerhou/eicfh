#ifndef DEFAULT_COST_MODEL_H
#define DEFAULT_COST_MODEL_H

#include "CostModel.h"
#include "Statistics.h"
#include "Weights.h"
#include <string>

namespace Halide {

class DefaultCostModel : public CostModel {
private:
    Internal::Weights weights;
    Runtime::Buffer<float> schedule_feat_queue, pipeline_feat_queue, costs, costs_per_stage;
    Runtime::Buffer<double *> cost_ptrs;
    std::vector<std::vector<double> *> cost_per_stage_ptrs;
    int cursor, num_stages, num_cores;
    int batch_id{0};

    const std::string weights_in_path, weights_out_path;
    const bool randomize_weights;

    Runtime::Buffer<float>
        head1_filter_update, head1_bias_update,
        head2_filter_update, head2_bias_update,
        conv1_filter_update, conv1_bias_update;
    int timestep = 0;

    Internal::Autoscheduler::Statistics &stats;

public:
    DefaultCostModel(const std::string &weights_in_path,
                     const std::string &weights_out_path,
                     bool randomize_weights,
                     Internal::Autoscheduler::Statistics &stats)
        : weights_in_path(weights_in_path),
          weights_out_path(weights_out_path),
          randomize_weights(randomize_weights),
          stats{stats} {
        load_weights();
    }

    // Configure the cost model for the algorithm to be scheduled.
    void set_pipeline_features(const Internal::Autoscheduler::FunctionDAG &dag,
                               const Internal::Autoscheduler::Anderson2021Params &params) override;
    void set_pipeline_features(const Runtime::Buffer<float> &, int n);

    // Enqueue a schedule to be evaluated. The second version of this method returns a buffer of
    // schedule_features that should be filled in by the caller.
    void enqueue(const Internal::Autoscheduler::FunctionDAG &dag,
                 const Halide::Internal::Autoscheduler::StageMapOfScheduleFeatures &schedule_feats,
                 double *cost_ptr,
                 std::vector<double> *cost_per_stage_ptr) override;
    void enqueue(int ns, Runtime::Buffer<float> *schedule_feats, double *cost_ptr, std::vector<double> *cost_per_stage_ptr);

    // Evaluate all schedules in the queue.
    void evaluate_costs() override;

    // Discard all schedules in the queue.
    void reset() override;

    // Update model weights using true measured runtimes.
    float backprop(const Runtime::Buffer<const float> &true_runtimes, float learning_rate);

    // Save/Load the model weights to/from disk.
    void save_weights();
    void load_weights();
};

std::unique_ptr<DefaultCostModel> make_default_cost_model(Internal::Autoscheduler::Statistics &stats,
                                                          const std::string &weights_in_dir = "",
                                                          const std::string &weights_out_dir = "",
                                                          bool randomize_weights = false);
}  // namespace Halide

#endif  // DEFAULT_COST_MODEL_H
