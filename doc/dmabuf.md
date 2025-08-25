https://wayland.app/protocols/linux-dmabuf-v1#zwp_linux_dmabuf_v1
zwp_linux_dmabuf_v1
version 5
factory for creating dmabuf-based wl_buffers
This interface offers ways to create generic dmabuf-based wl_buffers.

For more information about dmabuf, see: https://www.kernel.org/doc/html/next/userspace-api/dma-buf-alloc-exchange.html

Clients can use the get_surface_feedback request to get dmabuf feedback for a particular surface. If the client wants to retrieve feedback not tied to a surface, they can use the get_default_feedback request.

mesa : wsi -- waylandKHR
