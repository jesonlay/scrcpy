#ifndef VIDEO_BUFFER_H
#define VIDEO_BUFFER_H

#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_stdinc.h>

#include "config.h"
#include "fps_counter.h"

// forward declarations
typedef struct AVFrame AVFrame;

struct video_buffer {
    AVFrame *decoding_frame;
    AVFrame *rendering_frame;
    SDL_mutex *mutex;
#ifndef SKIP_FRAMES
    SDL_bool stopped;
    SDL_cond *rendering_frame_consumed_cond;
#endif
    SDL_bool rendering_frame_consumed;
    struct fps_counter fps_counter;
};

SDL_bool video_buffer_init(struct video_buffer *vb);
void video_buffer_destroy(struct video_buffer *vb);

// set the decoded frame as ready for rendering
// this function locks frames->mutex during its execution
// returns true if the previous frame had been consumed
SDL_bool video_buffer_offer_decoded_frame(struct video_buffer *vb);

// mark the rendering frame as consumed and return it
// MUST be called with frames->mutex locked!!!
// the caller is expected to render the returned frame to some texture before
// unlocking frames->mutex
const AVFrame *video_buffer_consume_rendered_frame(struct video_buffer *vb);

// wake up and avoid any blocking call
void video_buffer_stop(struct video_buffer *vb);

#endif