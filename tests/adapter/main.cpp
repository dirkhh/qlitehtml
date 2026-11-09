// SPDX-License-Identifier: BSD-3-Clause
#include <container_qpainter.h>

#include <QApplication>
#include <QBuffer>
#include <QImage>
#include <QPainter>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QImage image(320, 240, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    DocumentContainerContext context;
    DocumentContainer container;
    container.setPaletteCallback([] { return QPalette(); });
    container.setCursorCallback([](QCursor) {});
    container.setLinkCallback([](QUrl) {});
    container.setClipboardCallback([](bool) {});
    int requests = 0;
    QImage icon(2, 2, QImage::Format_ARGB32);
    icon.fill(Qt::red);
    QByteArray png;
    QBuffer buffer(&png);
    buffer.open(QIODevice::WriteOnly);
    icon.save(&buffer, "PNG");
    container.setDataCallback([&](QUrl) { ++requests; return png; });
    container.setPaintDevice(&image);
    container.setViewportSize(320, 240);
    container.setDocument("<style>a:hover { background: red }</style>"
                          "<a href='next'>Hover me</a><p id='anchor.with.dot'>Find me</p>"
                          "<img src='icon.png'>", &context);
    container.render(320, 240);
    if (container.documentWidth() <= 0 || container.anchorY("anchor.with.dot") < 0 || requests == 0)
        return 1;

    QPainter painter(&image);
    container.draw(&painter, image.rect());
    painter.end();

    // The engine now supplies invalidation rectangles through a callback, not a vector.
    if (container.mouseMoveEvent({10, 10}, {10, 10}).isEmpty())
        return 2;
    if (container.leaveEvent().isEmpty())
        return 3;

    bool wrapped = false;
    bool found = false;
    container.findText("Find me", {}, false, &wrapped, &found, nullptr, nullptr);
    return found ? 0 : 4;
}
