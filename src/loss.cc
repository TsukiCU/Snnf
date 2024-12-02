#include "loss.h"

namespace snnf {

/**
 * @brief: Forward pass
 */  
template <typename T>
T MSELoss<T>::forward(const Tensor<T>& input, const Tensor<T>& target) {
    input_ = input;
    target_ = target;

    // Compute MSE loss: (1/2N) * sum((input - target)^2)
    T loss = static_cast<T>(0);
    uint32_t N = input.size();
    for (uint32_t i = 0; i < N; ++i) {
        T diff = input.data()[i] - target.data()[i];
        loss += diff * diff;
    }
    loss /= static_cast<T>(2 * N);
    return loss;
}

/**
 * @brief: Backward pass
 */  
template <typename T>
Tensor<T> MSELoss<T>::backward() {
    // Compute gradient w.r.t input: grad_input = (input - target) / N
    uint32_t N = input_.size();
    Tensor<T> grad_input(input_.shape());
    for (uint32_t i = 0; i < N; ++i) {
        grad_input.data()[i] = (input_.data()[i] - target_.data()[i]) / static_cast<T>(N);
    }
    return grad_input;
}

/**
 * @brief: Forward pass. Not very precise (around 1e-3 precision.)
 */  
template <typename T>
T CrossEntropyLoss<T>::forward(const Tensor<T>& input, const Tensor<T>& target) {
    input_ = input;
    target_ = target;

    // Compute softmax
    softmax_ = input_;
    uint32_t batch_size = input.shape()[0];
    uint32_t num_classes = input.shape()[1];

    for (uint32_t i = 0; i < batch_size; ++i) {
        // Compute max value for numerical stability (Prevent exponetial overflow).
        T max_val = input.at({i, 0});
        for (uint32_t j = 1; j < num_classes; ++j) {
            T val = input.at({i, j});
            if (val > max_val) {
                max_val = val;
            }
        }

        T sum_exp = static_cast<T>(0);
        for (uint32_t j = 0; j < num_classes; ++j) {
            T exp_val = std::exp(input.at({i, j}) - max_val);
            softmax_.data()[i * num_classes + j] = exp_val;
            sum_exp += exp_val;
        }

        for (uint32_t j = 0; j < num_classes; ++j) {
            softmax_.data()[i * num_classes + j] /= sum_exp;
        }
    }

    // Compute cross-entropy loss: - (1/N) * sum(target * log(softmax))
    T loss = static_cast<T>(0);
    uint32_t N = batch_size;
    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < num_classes; ++j) {
            T target_val = target.at({i, j});
            if (target_val > static_cast<T>(0)) {
                T softmax_val = softmax_.at({i, j});
                loss -= target_val * std::log(softmax_val + static_cast<T>(1e-10));  // Add small value to avoid log(0)
            }
        }
    }
    loss /= static_cast<T>(N);
    return loss;
}

/**
 * @brief: Backward pass
 */  
template <typename T>
Tensor<T> CrossEntropyLoss<T>::backward() {
    // Compute gradient w.r.t input: grad_input = (softmax - target) / N
    uint32_t N = input_.shape()[0];
    uint32_t num_classes = input_.shape()[1];
    Tensor<T> grad_input(input_.shape());

    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < num_classes; ++j) {
            T grad = (softmax_.at({i, j}) - target_.at({i, j})) / static_cast<T>(N);
            grad_input.data()[i * num_classes + j] = grad;
        }
    }
    return grad_input;
}

// Explicit template instantiation
template class MSELoss<float>;
template class MSELoss<double>;
template class CrossEntropyLoss<float>;
template class CrossEntropyLoss<double>;

} // snnf