#include "interactor/validator/DefaultValidator.h"

#include <gtest/gtest.h>

#include <algorithm>

#include "test_utils/common.h"

class DefaultValidatorTest : public ::testing::Test
{
protected:
    DefaultValidator validator_;
};

TEST_F(DefaultValidatorTest, MustAcceptShortAndLongVariantsOfCommands)
{
    EXPECT_EQ(validator_.MakeRequest("add").first, CommandEnum::kAdd);
    EXPECT_EQ(validator_.MakeRequest("a").first, CommandEnum::kAdd);

    EXPECT_EQ(validator_.MakeRequest("edit").first, CommandEnum::kEdit);
    EXPECT_EQ(validator_.MakeRequest("e").first, CommandEnum::kEdit);

    EXPECT_EQ(validator_.MakeRequest("complete").first, CommandEnum::kComplete);
    EXPECT_EQ(validator_.MakeRequest("c").first, CommandEnum::kComplete);

    EXPECT_EQ(validator_.MakeRequest("delete").first, CommandEnum::kDelete);
    EXPECT_EQ(validator_.MakeRequest("d").first, CommandEnum::kDelete);

    EXPECT_EQ(validator_.MakeRequest("show").first, CommandEnum::kShow);
    EXPECT_EQ(validator_.MakeRequest("s").first, CommandEnum::kShow);

    EXPECT_EQ(validator_.MakeRequest("help").first, CommandEnum::kHelp);
    EXPECT_EQ(validator_.MakeRequest("h").first, CommandEnum::kHelp);

    EXPECT_EQ(validator_.MakeRequest("quit").first, CommandEnum::kQuit);
    EXPECT_EQ(validator_.MakeRequest("q").first, CommandEnum::kQuit);

    EXPECT_EQ(validator_.MakeRequest("add_label").first, CommandEnum::kAddLabel);
    EXPECT_EQ(validator_.MakeRequest("al").first, CommandEnum::kAddLabel);

    EXPECT_EQ(validator_.MakeRequest("delete_label").first,
              CommandEnum::kDeleteLabel);
    EXPECT_EQ(validator_.MakeRequest("dl").first, CommandEnum::kDeleteLabel);
}

TEST_F(DefaultValidatorTest, CommandsMustBeCaseInsensitive)
{
    EXPECT_EQ(validator_.MakeRequest("AdD").first, CommandEnum::kAdd);
    EXPECT_EQ(validator_.MakeRequest("EDit").first, CommandEnum::kEdit);
    EXPECT_EQ(validator_.MakeRequest("ComPLEte").first, CommandEnum::kComplete);
    EXPECT_EQ(validator_.MakeRequest("DeLEte").first, CommandEnum::kDelete);
    EXPECT_EQ(validator_.MakeRequest("SHow").first, CommandEnum::kShow);
    EXPECT_EQ(validator_.MakeRequest("HElp").first, CommandEnum::kHelp);
    EXPECT_EQ(validator_.MakeRequest("QUiT").first, CommandEnum::kQuit);
    EXPECT_EQ(validator_.MakeRequest("aDD_LaBel").first, CommandEnum::kAddLabel);
    EXPECT_EQ(validator_.MakeRequest("DeleTE_LaBel").first,
              CommandEnum::kDeleteLabel);
}

TEST_F(DefaultValidatorTest, EmptyTitleIsNullopt)
{
    EXPECT_EQ(validator_.ValidateTitle(""), std::nullopt);
}

TEST_F(DefaultValidatorTest, NonEmptyTitleShouldBeItself)
{
    std::string title = "fkjdhsaKHKSDFHSf9209vvdf/.,a'f[2-1";
    EXPECT_EQ(*validator_.ValidateTitle(title), title);
}

TEST_F(DefaultValidatorTest, PlusForCompletedMinusForUncompleted)
{
    EXPECT_EQ(*validator_.ParseTaskProgress("+"), Task::kCompleted);
    EXPECT_EQ(*validator_.ParseTaskProgress("-"), Task::kUncompleted);
}

TEST_F(DefaultValidatorTest, StringThatAreNotPlusOrMinusAreInvalidStates)
{
    EXPECT_EQ(validator_.ParseTaskProgress("y"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskProgress("n"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskProgress("completed"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskProgress("uncompleted"), std::nullopt);
}

TEST_F(DefaultValidatorTest, PriorityShouldBeLowMediumHigh)
{
    EXPECT_EQ(*validator_.ParseTaskPriority("low"), Task::kLow);
    EXPECT_EQ(*validator_.ParseTaskPriority("LoW"), Task::kLow);

    EXPECT_EQ(*validator_.ParseTaskPriority("medium"), Task::kMedium);
    EXPECT_EQ(*validator_.ParseTaskPriority("MEdIUm"), Task::kMedium);

    EXPECT_EQ(*validator_.ParseTaskPriority("high"), Task::kHigh);
    EXPECT_EQ(*validator_.ParseTaskPriority("HIgH"), Task::kHigh);
}

TEST_F(DefaultValidatorTest,
       StringThatAreNotLowMediumHighAreInvalidPriorities)
{
    EXPECT_EQ(validator_.ParseTaskPriority("0"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskPriority("small"), std::nullopt);

    EXPECT_EQ(validator_.ParseTaskPriority("1"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskPriority("intermediate"), std::nullopt);

    EXPECT_EQ(validator_.ParseTaskPriority("2"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskPriority("big"), std::nullopt);
}

TEST_F(DefaultValidatorTest, DateIsAcceptedOnlyInSingleFormat)
{
    EXPECT_NE(validator_.ParseTaskDate("12:23 10/12/2021"), std::nullopt);
    EXPECT_NE(validator_.ParseTaskDate("12:23 02/09/2021"), std::nullopt);

    EXPECT_EQ(validator_.ParseTaskDate(""), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("12:34"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("12:34 10/"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("12:34 10/12"), std::nullopt);

    EXPECT_EQ(validator_.ParseTaskDate("12:33 2/12/2021"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("12:3 23/09/2021"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("12:34 10/12/202"), std::nullopt);

    EXPECT_EQ(validator_.ParseTaskDate("12:95 10/12/2021"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("12:60 10/12/2021"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("25:59 10/12/2021"), std::nullopt);
    // EXPECT_EQ(validator_.ParseTaskDate("12:34 29/02/2021"), std::nullopt); //
    // TODO: Catch this
    EXPECT_EQ(validator_.ParseTaskDate("12:34 32/06/2021"), std::nullopt);
    EXPECT_EQ(validator_.ParseTaskDate("12:34 21/13/2021"), std::nullopt);
    //  EXPECT_EQ(validator_.ParseTaskDate("12:34 31/06/2021"), std::nullopt); //
    //  TODO: Catch this
}