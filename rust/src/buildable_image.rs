use crate::{image::RsGenericImage, system::path::RsPath};
use testcontainers::{runners::SyncBuilder, GenericBuildableImage};

pub struct RsGenericBuildableImage {
    pub image: GenericBuildableImage,
}

pub fn rs_generic_buildable_image_new(name: String, tag: String) -> Box<RsGenericBuildableImage> {
    Box::new(RsGenericBuildableImage::new(GenericBuildableImage::new(
        name, tag,
    )))
}

pub fn rs_generic_buildable_image_with_dockerfile(
    image: Box<RsGenericBuildableImage>,
    source: Box<RsPath>,
) -> Box<RsGenericBuildableImage> {
    Box::new(RsGenericBuildableImage::new(
        image.image.with_dockerfile(source.path),
    ))
}

pub fn rs_generic_buildable_image_with_dockerfile_string(
    image: Box<RsGenericBuildableImage>,
    content: String,
) -> Box<RsGenericBuildableImage> {
    Box::new(RsGenericBuildableImage::new(
        image.image.with_dockerfile_string(content),
    ))
}

pub fn rs_generic_buildable_image_with_file(
    image: Box<RsGenericBuildableImage>,
    source: Box<RsPath>,
    target: String,
) -> Box<RsGenericBuildableImage> {
    Box::new(RsGenericBuildableImage::new(
        image.image.with_file(source.path, target),
    ))
}

pub fn rs_generic_buildable_image_with_data(
    image: Box<RsGenericBuildableImage>,
    data: Vec<u8>,
    target: String,
) -> Box<RsGenericBuildableImage> {
    Box::new(RsGenericBuildableImage::new(
        image.image.with_data(data, target),
    ))
}

pub fn rs_generic_buildable_image_build(
    image: Box<RsGenericBuildableImage>,
) -> Result<Box<RsGenericImage>, String> {
    let built_image = image
        .image
        .build_image()
        .map_err(|e| format!("Failed to build image: {}", e))?;
    Ok(Box::new(RsGenericImage::new(built_image)))
}

pub fn rs_generic_buildable_image_destroy(image: Box<RsGenericBuildableImage>) {
    drop(image);
}

impl RsGenericBuildableImage {
    pub fn new(image: GenericBuildableImage) -> Self {
        Self { image }
    }
}
