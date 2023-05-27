CMAKE = cmake

define make-cmake-rule

$(1).cache:
	$(CMAKE) --preset conan-$(1)

$(1).build:
	$(CMAKE) --build --preset conan-$(1)

$(1).clean:
	$(CMAKE) --build --preset conan-$(1) --target clean

$(1).format:
	$(CMAKE) --build --preset conan-$(1) --target clang-format

$(1).format-fix:
	$(CMAKE) --build --preset conan-$(1) --target clang-format-fix

endef

BUILD_TYPES := debug release

$(foreach type, $(BUILD_TYPES), $(eval $(call make-cmake-rule,$(type))))
