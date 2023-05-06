#pragma once

#include "cws/subject/base.hpp"

enum class InteractionStateType {
  UNSPECIFIED = 0,
  OPENED = 1,
  CLOSED = 2,
};

struct InteractiveState {
  InteractionStateType type;
};

struct SubjectInteractive : public Subject {
  InteractiveState interactionState;

public:
  SubjectInteractive(int idx, SubjectParameters parameters, InteractiveState state)
      : Subject(SubjectId{SubjectType::INTERACTIVE, idx}, parameters),
        interactionState(state) {}

  SubjectInteractive * clone() const override { return new SubjectInteractive(*this); }

  InteractiveState getInteractionState() const { return interactionState; }

  void setInteractionState(InteractiveState interactionState) {
    this->interactionState = interactionState;
  }

  void nextState(SubjectId subjectId, const Layers & layers) override;
};
