use super::*;
use std::ffi::CString;
use std::fs::File;
use std::path::Path;

#[derive(Default, Debug)]
pub struct IPCkey {
    path: String,
    id_project: i32,
    pub id: i32,
}

impl IPCkey {
    pub fn set_path(&mut self, path: &str) {
        self.path = path.to_string();
    }

    pub fn set_project_id(&mut self, id_project: i32) {
        self.id_project = id_project;
    }

    pub fn keygen(&mut self, opt: &IPCInitOption) -> Result<(), BoxError> {
        // Если IPCInitOption::Join, то должны проверить, что файл для генерации ключа существует.
        // В противном случае, отсутствие файла будет свидетельствовать о том, что другой процесс
        // НЕ инициализировал System V IPC.
        if opt == &IPCInitOption::Join && Path::new(&self.path).exists() == false {
            return Err(format!("E! File {} doesn't exists.", self.path).into());
        }

        // Если IPCInitOption::Create, то должны проверить, что файла для генерации ключа не существует.
        // В противном случае, присутствие файла будет свидетельствовать о том, что другой процесс
        // инициализировал или начал инициализировать System V IPC.
        if opt == &IPCInitOption::Create && Path::new(&self.path).exists() == true {
            return Err(format!("E! File {} already exists.", self.path).into());
        } else {
            File::create(&self.path)?;
        }

        // Генерируем ключ.
        let file = CString::new(self.path.as_bytes().to_vec())?;
        self.id = unsafe { ftok(file.as_ptr(), self.id_project) };
        if let -1 = self.id {
            return Err(Box::new(Error::last_os_error()));
        }
        Ok(())
    }
}

// /// Удалить созданный файл
// pub fn clean(&mut self) {
//     match fs::remove_file(&self.path) {
//         Ok(()) => {
//             eprintln!("key file: '{}' deleted successfully", self.path);
//         }
//         Err(_) => {
//             eprintln!("key file: '{}' unable to delete file", self.path);
//         }
//     }
// }
