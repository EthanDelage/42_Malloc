ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME        := ft_malloc_$(HOSTTYPE)
BUILD_DIR   := .build
TEST_BUILD  := $(BUILD_DIR)/test

.PHONY: all
all: $(NAME)

$(NAME):
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target $(NAME)

.PHONY: clean
clean:
	$(MAKE) -C $(BUILD_DIR) clean || true
	$(MAKE) -C $(TEST_BUILD) clean || true

.PHONY: fclean
fclean:
	rm -rf $(BUILD_DIR)

.PHONY: re
re: fclean all

.PHONY: test
test:
	cmake -S . -B $(TEST_BUILD) -DBUILD_TESTS=ON
	cmake --build $(TEST_BUILD)
	ctest --test-dir $(TEST_BUILD) --output-on-failure

.PHONY: format
format:
	git ls-files "*.c" "*.h" | xargs clang-format -i
