#include <memory>
#include "execution/plans/abstract_plan.h"
#include "execution/plans/topn_plan.h"
#include "execution/plans/limit_plan.h"
#include "execution/plans/sort_plan.h"
#include "optimizer/optimizer.h"

namespace bustub {

auto Optimizer::OptimizeSortLimitAsTopN(const AbstractPlanNodeRef &plan) -> AbstractPlanNodeRef {
  // TODO(student): implement sort + limit -> top N optimizer rule
  std::vector<AbstractPlanNodeRef> children;
  for (const auto &child : plan->GetChildren()) {
    children.emplace_back(OptimizeSortLimitAsTopN(child));
  }

  auto optimized_plan = plan->CloneWithChildren(std::move(children));
  if (optimized_plan->GetType() == PlanType::Limit && optimized_plan->GetChildAt(0)->GetType() == PlanType::Sort) {
   const auto &limit_plan = dynamic_cast<const LimitPlanNode &>(*optimized_plan);
   const auto &sort_plan = dynamic_cast<const SortPlanNode &>(*limit_plan.GetChildAt(0));
   return std::make_shared<TopNPlanNode>(limit_plan.output_schema_, sort_plan.GetChildPlan(), sort_plan.GetOrderBy(), limit_plan.GetLimit());
  }

  return optimized_plan;
}

}  // namespace bustub
