//
// Copyright (c) 2018, University of Edinburgh
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of  nor the names of its contributors may be used to
//    endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef EXOTICA_CORE_TASKS_H_
#define EXOTICA_CORE_TASKS_H_

#include <Eigen/Dense>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <exotica_core/planning_problem.h>
#include <exotica_core/property.h>
#include <exotica_core/task_map.h>
#include <exotica_core/task_space_vector.h>
#include <exotica_core/tools/conversions.h>

namespace exotica
{
struct TaskIndexing
{
    int id;
    int start;
    int length;
    int start_jacobian;
    int length_jacobian;
};

struct Task
{
    Task();

    virtual void Initialize(const std::vector<exotica::Initializer>& inits, std::shared_ptr<PlanningProblem> prob, TaskSpaceVector& Phi);

    TaskMapMap task_maps;
    TaskMapVec tasks;
    std::vector<TaskIndexing> indexing;

    int length_phi;
    int length_jacobian;
    int num_tasks;
    double tolerance = 0.0;  // To avoid numerical issues consider all values below this as 0.0.

protected:
    std::vector<TaskInitializer> task_initializers_;
};

struct TimeIndexedTask : public Task
{
    TimeIndexedTask();
    virtual void Initialize(const std::vector<exotica::Initializer>& inits, std::shared_ptr<PlanningProblem> prob, TaskSpaceVector& Phi);
    void UpdateS();
    void Update(const TaskSpaceVector& big_phi, Eigen::MatrixXdRefConst big_j, HessianRefConst big_h, int t);
    void Update(const TaskSpaceVector& big_phi, Eigen::MatrixXdRefConst big_j, int t);
    void Update(const TaskSpaceVector& big_phi, int t);
    void ReinitializeVariables(int _T, std::shared_ptr<PlanningProblem> _prob, const TaskSpaceVector& _phi);

    std::vector<Eigen::VectorXd> rho;
    std::vector<TaskSpaceVector> y;
    std::vector<Eigen::VectorXd> ydiff;
    std::vector<TaskSpaceVector> Phi;
    std::vector<Hessian> hessian;
    std::vector<Eigen::MatrixXd> jacobian;
    std::vector<Eigen::MatrixXd> S;
    int T;
};

struct EndPoseTask : public Task
{
    EndPoseTask();
    virtual void Initialize(const std::vector<exotica::Initializer>& inits, std::shared_ptr<PlanningProblem> prob, TaskSpaceVector& Phi);
    void UpdateS();
    void Update(const TaskSpaceVector& big_phi, Eigen::MatrixXdRefConst big_j, HessianRefConst big_h);
    void Update(const TaskSpaceVector& big_phi, Eigen::MatrixXdRefConst big_j);
    void Update(const TaskSpaceVector& big_phi);

    Eigen::VectorXd rho;
    TaskSpaceVector y;
    Eigen::VectorXd ydiff;
    TaskSpaceVector Phi;
    Eigen::MatrixXd jacobian;
    Hessian hessian;
    Eigen::MatrixXd S;
};

struct SamplingTask : public Task
{
    SamplingTask();
    virtual void Initialize(const std::vector<exotica::Initializer>& inits, std::shared_ptr<PlanningProblem> prob, TaskSpaceVector& Phi);
    void UpdateS();
    void Update(const TaskSpaceVector& big_phi);

    Eigen::VectorXd rho;
    TaskSpaceVector y;
    Eigen::VectorXd ydiff;
    TaskSpaceVector Phi;
    Eigen::MatrixXd S;
};
}

#endif  // EXOTICA_CORE_TASKS_H_
