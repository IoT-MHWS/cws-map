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
  int id;
  InteractiveState interactionState;

public:
  SubjectInteractive(int id, SubjectParameters parameters, InteractiveState state)
      : Subject(SubjectType::INTERACTIVE, parameters), id(id),
        interactionState(state) {}

  SubjectInteractive * clone() const override { return new SubjectInteractive(*this); }

  int getId() const { return id; }

  InteractiveState getInteractionState() const { return interactionState; }

  void setInteractionState(InteractiveState interactionState) {
    this->interactionState = interactionState;
  }

  void nextState(SubjectId subjectId, const Layers & layers) override;
};
