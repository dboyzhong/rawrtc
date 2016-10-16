#include <netinet/in.h> // IPPROTO_UDP, IPPROTO_TCP
#include <stdarg.h>
#include <anyrtc.h>
#include "utils.h"

struct anyrtc_config anyrtc_default_config = {
        .pacing_interval = 20,
        .ipv4_enable = true,
        .ipv6_enable = false, // TODO: true by default
        .udp_enable = true,
        .tcp_enable = false // TODO: true by default
};

/*
 * Translate an re error to an anyrtc code.
 * TODO: Add codes from trice_lcand_add
 */
enum anyrtc_code anyrtc_translate_re_code(
        int code
) {
    switch (code) {
        case 0:
            return ANYRTC_CODE_SUCCESS;
        case EINVAL:
            return ANYRTC_CODE_INVALID_ARGUMENT;
        case ENOMEM:
            return ANYRTC_CODE_NO_MEMORY;
        default:
            return ANYRTC_CODE_UNKNOWN_ERROR;
    }
}

/*
 * Translate a protocol to the corresponding IPPROTO_*.
 */
int anyrtc_translate_ice_protocol(
        enum anyrtc_ice_protocol const protocol
) {
    // No conversion needed
    return (int) protocol;
}

/*
 * Translate a IPPROTO_* to the corresponding protocol.
 */
enum anyrtc_code anyrtc_translate_ipproto(
        int const ipproto,
        enum anyrtc_ice_protocol* const protocolp // de-referenced
) {
    // Check arguments
    if (!protocolp) {
        return ANYRTC_CODE_INVALID_ARGUMENT;
    }

    // Convert IPPROTO_*
    switch (ipproto) {
        case IPPROTO_UDP:
            *protocolp = ANYRTC_ICE_PROTOCOL_UDP;
            return ANYRTC_CODE_SUCCESS;
        case IPPROTO_TCP:
            *protocolp = ANYRTC_ICE_PROTOCOL_TCP;
            return ANYRTC_CODE_SUCCESS;
        default:
            return ANYRTC_CODE_INVALID_ARGUMENT;
    }
}

enum ice_cand_type anyrtc_translate_ice_candidate_type(
        enum anyrtc_ice_candidate_type type
) {
    // No conversion needed
    return (enum ice_cand_type) type;
}

enum anyrtc_code anyrtc_translate_re_ice_cand_type(
        enum ice_cand_type re_type,
        enum anyrtc_ice_candidate_type* const typep // de-referenced
) {
    // Check arguments
    if (!typep) {
        return ANYRTC_CODE_INVALID_ARGUMENT;
    }

    // Convert ice_cand_type
    switch (re_type) {
        case ICE_CAND_TYPE_HOST:
            *typep = ANYRTC_ICE_CANDIDATE_TYPE_HOST;
            return ANYRTC_CODE_SUCCESS;
        case ICE_CAND_TYPE_SRFLX:
            *typep = ANYRTC_ICE_CANDIDATE_TYPE_SRFLX;
            return ANYRTC_CODE_SUCCESS;
        case ICE_CAND_TYPE_PRFLX:
            *typep = ANYRTC_ICE_CANDIDATE_TYPE_PRFLX;
            return ANYRTC_CODE_SUCCESS;
        case ICE_CAND_TYPE_RELAY:
            *typep = ANYRTC_ICE_CANDIDATE_TYPE_RELAY;
            return ANYRTC_CODE_SUCCESS;
        default:
            return ANYRTC_CODE_INVALID_ARGUMENT;
    }
}

enum ice_tcptype anyrtc_translate_ice_tcp_candidate_type(
        enum anyrtc_ice_tcp_candidate_type type
) {
    // No conversion needed
    return (enum ice_tcptype) type;
}

enum anyrtc_code anyrtc_translate_re_ice_tcptype(
        enum ice_tcptype re_type,
        enum anyrtc_ice_tcp_candidate_type* const typep // de-referenced
) {
    // Check arguments
    if (!typep) {
        return ANYRTC_CODE_INVALID_ARGUMENT;
    }

    // Convert ice_cand_type
    switch (re_type) {
        case ICE_TCP_ACTIVE:
            *typep = ANYRTC_ICE_TCP_CANDIDATE_TYPE_ACTIVE;
            return ANYRTC_CODE_SUCCESS;
        case ICE_TCP_PASSIVE:
            *typep = ANYRTC_ICE_TCP_CANDIDATE_TYPE_PASSIVE;
            return ANYRTC_CODE_SUCCESS;
        case ICE_TCP_SO:
            *typep = ANYRTC_ICE_TCP_CANDIDATE_TYPE_SO;
            return ANYRTC_CODE_SUCCESS;
        default:
            return ANYRTC_CODE_INVALID_ARGUMENT;
    }
}

enum anyrtc_code anyrtc_strdup(
        char** const destination,
        char const * const source
) {
    int err = str_dup(destination, source);
    return anyrtc_translate_re_code(err);
}

enum anyrtc_code anyrtc_snprintf(
        char* const destination,
        size_t const size,
        char* const formatter,
        ...
) {
    va_list args;
    va_start(args, formatter);
    int err = re_vsnprintf(destination, size, formatter, args);
    va_end(args);

    // For some reason, re_vsnprintf does return -1 on argument error
    switch (err) {
        case -1:
            return ANYRTC_CODE_INVALID_ARGUMENT;
        default:
            return anyrtc_translate_re_code(err);
    }
}

enum anyrtc_code anyrtc_sdprintf(
        char** const destinationp,
        char* const formatter,
        ...
) {
    va_list args;
    va_start(args, formatter);
    int err = re_vsdprintf(destinationp, formatter, args);
    va_end(args);
    return anyrtc_translate_re_code(err);
}

